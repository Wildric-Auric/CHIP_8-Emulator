#pragma once
#include "global.h"
#include <map>

class Keyboard {
public:
    std::map<uint8, uint8> keys = {
        {0x1, GLFW_KEY_1}, // 1
        {0x2, GLFW_KEY_2}, // 2
        {0x3, GLFW_KEY_3}, // 3
        {0xc, GLFW_KEY_4}, // 4
        {0x4, GLFW_KEY_Q}, // Q
        {0x5, GLFW_KEY_W}, // W
        {0x6, GLFW_KEY_E}, // E
        {0xD, GLFW_KEY_R}, // R
        {0x7, GLFW_KEY_A}, // A
        {0x8, GLFW_KEY_S}, // S
        {0x9, GLFW_KEY_D}, // D
        {0xE, GLFW_KEY_F}, // F
        {0xA, GLFW_KEY_Z}, // Z
        {0x0, GLFW_KEY_X}, // X
        {0xB, GLFW_KEY_C}, // C
        {0xF, GLFW_KEY_V}  // V
    };
    bool isPressed(uint8 key, GLFWwindow* window) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    };
};

class Speaker {
public:
    void Play() {
        //Play a 440 HZ sound
    }
    void Stop() {

    };
};

