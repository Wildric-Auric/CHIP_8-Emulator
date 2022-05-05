#pragma once
#include "global.h"

class Renderer {
private:
    GLFWwindow* InitContext(int scrWidth, int scrHeight);
public:
    uint8 frameBuffer[64 * 32] = { 0 };
    GLFWwindow* window;

    Renderer() {
        window = InitContext(640, 320);
    }

    bool SetPixel(int x, int y);

    void Clear();
};








