#include "Renderer.h"
#include <iostream>

void sizeCallBack(GLFWwindow* window, int width, int height)
{
	//WINDOW_WIDTH = width;
	//WINDOW_HEIGHT = height;
	//RENDERING_WIDTH = (int)(height * SCREENRATIO);
	//RENDERING_HEIGHT = height;
	//glViewport(0, 0, ORIGINAL_WIDTH, ORIGINAL_HEIGHT);
}

GLFWwindow* Renderer::InitContext(int scrWidth, int scrHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(scrWidth, scrHeight, "NWengine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to init glfw window";
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, sizeCallBack);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW";
		return 0;
	}
	return window;
}



void Renderer::Clear(){
		for (int i = 0; i < 64 * 32; i++) frameBuffer[i] = 0; 	 //It's slow
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}


bool Renderer::SetPixel(int x, int y) {
	//Clamping first
	if (x >= 64) x -= 64;
	else if (x < 0) x += 64;

	if (y >= 32) y -= 32;
	else if (y < 0) y += 32;

	uint16 location = x + y * 64;

	this->frameBuffer[location] = 1 ^ this->frameBuffer[location];
	glScissor(10*x, 320 - 10*y, 10, 10);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // (or whatever buffer you want to clear)
	glDisable(GL_SCISSOR_TEST);
	return !this->frameBuffer[location];
}
