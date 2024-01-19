#pragma once
#include <GL\glew.h>
#include <GLM/glm.hpp>

class Light
{
public:
	Light();

	// aIntensity : ambient intensity || this is without diffusion
	//Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

	// dIntensity : diffuse intensity || with Diffusion
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	// without diffusion || used to assign light data in the shader
	//void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	// with diffusion
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, 
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

