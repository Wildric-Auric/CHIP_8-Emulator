#include "CPU.h"


int CPU::LoadRom(std::string name) {
    FILE* rom;
    fopen_s(&rom, (std::string("./roms/") + name).c_str(), "r");
    if (!rom) {
        std::cout << "ERROR::FAILED TO OPEN ROM" << std::endl;
        return -1;
    };
    fseek(rom, 0, SEEK_END);
    uint16 size = ftell(rom);
    rewind(rom);

    if (size > 0xFFF - 0x200) {
        std::cout << "ERROR::UNSUPPORTED LARGE ROM" << std::endl;
        fclose(rom);
        return -1;
    }

    char* buffer = (char*)malloc(8 * size);

    if (!buffer) {
        std::cout << "ERROR::ALLOCATION FOR ROM FAILED" << std::endl;
        fclose(rom);
        return -1;
    }

    if (fread(buffer, 1, size, rom) != size) {
        std::cout << "ERROR::FAILED TO READ ROM" << std::endl;
        fclose(rom);
        return -1;
    };
    //Copying before into memory before deleting it
    memcpy(&this->memory[0x200], buffer, size);
    //for (int i = 0; i < size; i++) this->memory[0x200 + i] = buffer[i];

    fclose(rom);
    free(buffer);

    return 0;
}


void CPU::Cycle() {
        uint16 opcode = (this->memory[this->programCounter] << 8) | this->memory[this->programCounter + 1];
        
        //Execute instruction
        this->programCounter += 2;
        uint8 x = (opcode & 0x0F00) >> 8;
        uint8 y = (opcode & 0x00F0) >> 4;
        //Debug
        std::cout << "Opcode is: " << opcode << " From Memory Adress " <<(int) this->programCounter << std::endl;

        switch (opcode & 0xF000) {
            case 0x0000: {
                switch (opcode) {
                    case 0x00E0: {
                        this->renderer->Clear();
                    } break;
                    case 0x00EE: {
                        this->programCounter = this->stack[this->sPtr];
                        this->stack[this->sPtr] = -1;
                        sPtr -= 1;
                    } break;
                }
            } break;
            case 0x1000: {
                this->programCounter = (opcode & 0xFFF);
            } break;

            case 0x2000: {
                sPtr += 1;
                this->stack[sPtr] = this->programCounter;
                this->programCounter = (opcode & 0xFFF);
            } break;

            case 0x3000: {
                if (this->reg[x] == (opcode & 0xFF)) {
                    this->programCounter += 2;
                }
            } break;

            case 0x4000: {
                if (this->reg[x] != (opcode & 0xFF)) {
                    this->programCounter += 2;
                }
            } break;

            case 0x5000: {
                if (this->reg[x] == this->reg[y]) {
                    this->programCounter += 2;
                }
            } break;

            case 0x6000: {
                this->reg[x] = (opcode & 0xFF);
            } break;


            case 0x7000: {
                this->reg[x] += (opcode & 0xFF);
            } break;

            case 0x8000: {
                switch (opcode & 0xF) {
                case 0x0: {
                    this->reg[x] = this->reg[y];
                } break;

                case 0x1: {
                    this->reg[x] = this->reg[x] | this->reg[y];
                } break;

                case 0x2: {
                    this->reg[x] = this->reg[x] & this->reg[y];
                } break;

                case 0x3: {
                    this->reg[x] = this->reg[x] ^ this->reg[y];
                } break;

                case 0x4: {
                    uint16 temp = this->reg[x] + this->reg[y];
                    this->reg[0xF] = 0;
                    if (temp > 0xFF) {
                        this->reg[0xF] = 1;  
                    }
                    this->reg[x] = temp & 0xF;   //WARNING:: Maybe mistake
                    
                } break;

                case 0x5: {
                    this->reg[0xF] = 0;
                    if (this->reg[x] > this->reg[y]) {
                        this->reg[0xF] = 1;
                        this->reg[x] -= this->reg[y];
                    }
                    else this->reg[x] = 255 - (this->reg[y] - this->reg[x]);
                } break;

                case 0x6: {
                    this->reg[0xF] = (this->reg[x] & 0x1);
                    this->reg[x] = this->reg[x] >> 1;
                } break;

                case 0x7: {
                    this->reg[0xF] = 0;
                    if (this->reg[y] > this->reg[x]) {
                        this->reg[0xF] = 1;
                        this->reg[x] = this->reg[y] - this->reg[x];
                    }
                    else this->reg[x] = 255 - (this->reg[x] - this->reg[y]);

                } break;

                case 0xE: {
                    this->reg[x] = (this->reg[x] & 0x80);
                    this->reg[x] <<= 1;
                } break;
                }
            } break;

            case 0x9000: {
                if (this->reg[x] != this->reg[y])
                    this->programCounter += 2;
            } break;

            case 0xA000: {
                this->indexReg = opcode & 0xFFF;
            } break;
            case 0xB000: {
                this->programCounter = (opcode & 0xFFF) + this->reg[0];
            } break;
            case 0xC000: {
                this->reg[x] = (uint8)(rand() / RAND_MAX * 0xFF) & (opcode & 0xFF);
            } break;

            case 0xD000: {
                uint8 width = 8;
                uint8 height = opcode & 0xF;
                this->reg[0xF] = 0;

                for (uint8 row = 0; row < height; row++) {
                    uint8 sprite = this->memory[this->indexReg + row];
                    for (uint8 col = 0; col < width; col++) {
                        if ((sprite & 0x80) > 0) {
                            if (this->renderer->SetPixel(this->reg[x] + col, this->reg[y] + row))
                                this->reg[0xF] = 1;
                        }
                        sprite <<= 1;
                    }
                }

            } break;

            case 0xE000: {
                switch (opcode & 0xFF) {
                case 0x9E: {
                    if (this->keyboard->isPressed(this->reg[x], this->renderer->window))
                        this->programCounter += 2;
                } break;

                case 0xA1: {
                    if (!this->keyboard->isPressed(this->reg[x], this->renderer->window));
                    this->programCounter += 2;
                } break;

                }

            } break;
            case 0xF000: {
                switch (opcode & 0xFF) {
                    case 0x07: {
                        this->reg[x] = this->delayTimer;
                    } break;

                    case 0x0A: {
                        bool br = 1;
                        while (br) {
                            for (auto key : this->keyboard->keys) {
                                if (this->keyboard->isPressed(key.second, this->renderer->window))
                                    br = 0;
                            } break;
                        }
                    }
                } break;
                case 0x15: {
                    this->delayTimer = this->reg[x];
                } break;

                case 0x18: {
                    this->soundTimer = this->reg[x];
                } break;

                case 0x1E: {
                    this->indexReg += this->reg[x];
                } break;

                case 0x29: {
                    this->indexReg = this->reg[x] * 5;
                } break;

                case 0x33: {
                    this->memory[this->indexReg] = (uint8)(this->reg[x] / 100);
                    this->memory[this->indexReg + 1] = (uint8)(this->reg[x] % 100 / 10);
                    this->memory[this->indexReg + 2] = (uint8)(this->reg[x] % 10);
                } break;

                case 0x55: {

                } break;

                case 0x65: {
                    for (uint8 i = 0; i <= x; i++)
                        this->reg[i] = this->memory[this->indexReg + i];
                } break;
            }
            default: {
                std::cout<< "ERROR::UNKNOWN OPCODE: " << opcode  <<" FROM: "<< (int)this->memory[this->programCounter] << " "<<(int)this->memory[this->programCounter+1] <<
                    " AT: "<< (int) this->programCounter <<  std::endl;
            }
                   break;
        } 
}


void CPU::UpdateTimers() {
    if (delayTimer > 0) delayTimer -= 1;
    if (soundTimer > 0) soundTimer -= 1;
    if (this->soundTimer > 0) {
        this->speaker->Play();
    }
    else {
        this->speaker->Stop();
    }
}