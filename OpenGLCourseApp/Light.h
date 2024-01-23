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
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	// without diffusion || used to assign light data in the shader
	//void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

