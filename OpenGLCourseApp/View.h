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

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~View();

private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//Keyboard
	bool keys[1024];
	
	//Mouse
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();

	static void handleKeys(GLFWwindow* view, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* view, double xPos, double yPos);

};

