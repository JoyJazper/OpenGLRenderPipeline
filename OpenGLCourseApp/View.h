#pragma once

#include <stdio.h>

#include<GL\glew.h>
#include<GLFW/glfw3.h>

class View
{

public:
	View();
	
	View(GLint windowHeight, GLint windowWidth);
	
	int initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~View();

private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

