#include "CPU.h"


int main(){
    Renderer r = Renderer();
    Keyboard k = Keyboard();
    Speaker  s = Speaker();

    CPU* cpu = new CPU(&r, &k, &s);
    
    if (cpu->LoadRom("BLITZ.rom") == -1) return 0 ;
    
    float last = glfwGetTime() - 1;
    float last0 = glfwGetTime();

    while (!glfwWindowShouldClose(cpu->renderer->window)) {
        float elapsed = glfwGetTime() - last;
        float timeUp = glfwGetTime() - last0;
        if (elapsed >= 0.016) {
            last = glfwGetTime();
            cpu->Cycle();
            glfwSwapBuffers(cpu->renderer->window);
            glfwPollEvents();
        }
        if (timeUp > 1.0f) {
            last0 = 0.0f;
            cpu->UpdateTimers();
        }
        
    };
    delete cpu;
    return 0;
}