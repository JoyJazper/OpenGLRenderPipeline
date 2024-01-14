#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/mat4x4.hpp>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "View.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
View mainWindow;
int fps = 0;
float curAngle = 0.0f;
float triOffset = 0.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

glm::mat4 model = glm::mat4(1.0f);

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() 
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	Mesh *pyramid = new Mesh();
	pyramid->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(pyramid);
}

void CreateShader() {
	Shader *shader1 = new Shader();
	//shader1->CreateFromString(vShader, fShader);
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() 
{
	mainWindow = View(WIDTH, HEIGHT);
	mainWindow.initialise();

	CreateObjects();
	CreateShader();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) 
	{
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Get + Handle user input event
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);

		curAngle += 1.0f;
		if (curAngle >= 360.0f) {
			curAngle -= 360.0f;
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		glUseProgram(0);

		// uses 2 buffers one to draw in and one to show
		mainWindow.swapBuffers();
	}
	return 0;
}