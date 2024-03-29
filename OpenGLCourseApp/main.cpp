#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GLM/mat4x4.hpp>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


#include "Mesh.h"
#include "Shader.h"
#include "View.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "DirectionLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

// Window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;
View mainWindow;
int fps = 0;
float curAngle = 0.0f;
float triOffset = 0.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brick;
Texture dirt;

Material shiny;
Material dull;

DirectionLight directionLight;

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

void CalcAverageNormal(unsigned int * indices, unsigned int indiceCount, 
	GLfloat * vertices, unsigned int verticeCount, 
	unsigned int vLength, unsigned int normalOffset) 
{
	for(size_t i = 0; i < indiceCount; i += 3)
	{
		// getting 3 vertices
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i+1] * vLength;
		unsigned int in2 = indices[i+2] * vLength;
		// making 2 lines using 3 vertices
		// line 1
		glm::vec3 v1(vertices[in1] - vertices[in0], 
			vertices[in1 + 1] - vertices[in0 + 1], 
			vertices[in1 + 2] - vertices[in0 + 2]);
		// line 2
		glm::vec3 v2(vertices[in2] - vertices[in0],
			vertices[in2 + 1] - vertices[in0 + 1],
			vertices[in2 + 2] - vertices[in0 + 2]);

		// making the normal vector to the 2 lines using cross product 
		glm::vec3 normal = glm::cross(v1, v2);
		// getting just direction
		normal = glm::normalize(normal);

		// getting the vertices position for normal to set values.
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		// setting normal values
		vertices[in0] += normal.x;	vertices[in0 + 1] += normal.y;	vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;	vertices[in1 + 1] += normal.y;	vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;	vertices[in2 + 1] += normal.y;	vertices[in2 + 2] += normal.z;
	}

	// normalizing the assigned normals 
	for(size_t i = 0; i < verticeCount/vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;	vertices[nOffset + 1] = vec.y;	vertices[nOffset + 2] = vec.z;
	}
}

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
		-1.0f, -1.0f, -0.6f,

		// texture coordinates 1:   u   v 
		0.0f, 0.0f,

		// normal vector nx  ny  nz
		0.0f, 0.0f, 0.0f,

		// vertex 2
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		// vertex 3
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		// vertex 4
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalcAverageNormal(indices, 12, vertices, 32, 8, 5);

	Mesh *pyramid1 = new Mesh();
	pyramid1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(pyramid1);

	Mesh *pyramid2 = new Mesh();
	pyramid2->CreateMesh(vertices, indices, 32, 12);
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
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 3.0f);

	brick = Texture(brickLocation);
	brick.LoadTexture();
	dirt = Texture(dirtLocation);
	dirt.LoadTexture();

	shiny = Material(1.0f, 32);
	dull = Material(0.3f, 4);
	
	directionLight = DirectionLight(1.0f, 1.0f, 1.0f,   //colour     
									0.2f, 0.3f,			//intensities ambient and diffuse
									0.0f, 0.0f, -1.0f); //direction

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformAmbientIntensity = 0;
	GLuint uniformAmbientColour = 0;
	GLuint uniformDirection = 0;
	GLuint uniformDiffuseIntensity = 0;
	GLuint uniformEyePosition = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)(mainWindow.getBufferWidth() / mainWindow.getBufferHeight()), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) 
	{
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Get + Handle user input event
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

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
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		directionLight.UseDirectionLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brick.UseTexture();
		shiny.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirt.UseTexture();
		dull.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glUseProgram(0);

		// uses 2 buffers one to draw in and one to show
		mainWindow.swapBuffers();
	}
	return 0;
}