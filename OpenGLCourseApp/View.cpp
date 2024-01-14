#include "View.h"

View::View() {
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = false;
}

View::View(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = false;
}

int View::initialise() {
	// Initializing GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle key and mouse
	createCallbacks();

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!!!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

}

void View::createCallbacks() 
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat View::getXChange() 
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat View::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void View::handleKeys(GLFWwindow* view, int key, int code, int action, int mode) 
{
	View* theView = static_cast<View*>(glfwGetWindowUserPointer(view));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(view, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theView->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE) {
			theView->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void View::handleMouse(GLFWwindow* view, double xPos, double yPos) 
{
	View* theView = static_cast<View*>(glfwGetWindowUserPointer(view));

	if (theView->mouseFirstMoved) {
		theView->lastX = xPos;
		theView->lastY = yPos;
		theView->mouseFirstMoved = false;
	}

	theView->xChange = xPos - theView->lastX;
	theView->yChange = theView->lastY - yPos;

	theView->lastX = xPos;
	theView->lastY = yPos;

	//printf("x: %.6f, y: %.6f\n", theView->xChange, theView->yChange);
}

View::~View() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
