#define STB_IMAGE_IMPLEMENTATION

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
#include "Texture.h"

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

Texture brick;
Texture dirt;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

glm::mat4 model = glm::mat4(1.0f);

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

// Vertex shader
const char* brickLocation = "Textures/brick.png";

// Fragment shader
const char* dirtLocation = "Textures/dirt.png";

void CreateObjects() 
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	// changing this will need change in attributes section of mesh.cpp
	GLfloat vertices[] = {
		// vertex 1:    x  y  z
		-1.0f, -1.0f, 0.0f,
		// texture coordinates 1:   u   v 
		0.0f, 0.0f,
		// vertex 2
		0.0f, -1.0f, 1.0f,
		// texture coordinates 2
		0.5f, 0.0f,

		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
	};

	Mesh *pyramid1 = new Mesh();
	pyramid1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(pyramid1);

	Mesh* pyramid2 = new Mesh();
	pyramid2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(pyramid2);
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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.01f);

	brick = Texture(brickLocation);
	brick.LoadTexture();
	dirt = Texture(dirtLocation);
	dirt.LoadTexture();

	brick.UseTexture();

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
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

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
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brick.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirt.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);

		// uses 2 buffers one to draw in and one to show
		mainWindow.swapBuffers();
	}
	return 0;
}