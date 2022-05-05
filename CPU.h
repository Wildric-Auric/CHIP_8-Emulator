#pragma once
#include "global.h"
#include <string>
#include <random>
#include "Keyboard.h"
#include "Renderer.h"

/* source: Cowgod
+-------------- - += 0xFFF (4095) End of Chip - 8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip - 8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+--------+= 0x600 (1536) Start of ETI 660 Chip - 8 programs
|               |
|               |
|               |
+-------------- - += 0x200 (512) Start of most Chip - 8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter |
+-------------- - += 0x000 (0) Start of Chip - 8 RAM
*/

class CPU {
public:
    uint8 memory[4096]; //4096 bytes of memory; supported programs start at 0x200														
    uint8 reg[16];      //16 register
    uint16 indexReg = 0;
    uint8 delayTimer = 0;
    uint8 soundTimer = 0;
    uint16 programCounter = 0x200;
    uint8 stack[65];
    uint8 sPtr = 0; //Stack pointer


    Renderer* renderer;
    Keyboard* keyboard;
    Speaker* speaker;

    CPU(Renderer* r, Keyboard* k, Speaker* s) {
        renderer = r;
        keyboard = k;
        speaker = s;

        //Loading binary representation (sprites) as hex
        uint8 sprites[80] = {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
        memcpy(this->memory, &sprites[0], 80); //From 0x00 to 0x1FF 640 bits <-> 80 bytes
    }

    int LoadRom(std::string name);

    void Cycle();

    void UpdateTimers();
};
/*

0x 1   2   3   4
       _________Aadrr, nnn
               _Nibble, n
       _x -
           _y -
           _____kk, byte -
*/
