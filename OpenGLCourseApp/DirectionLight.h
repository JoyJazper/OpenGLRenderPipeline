#pragma once

#include "Light.h"

class DirectionLight : public Light
{
public:
	DirectionLight();
	~DirectionLight();

	// dIntensity : diffuse intensity || with Diffusion
	DirectionLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);

	// without diffusion || used to assign light data in the shader
	//void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	// with diffusion
	void UseDirectionLight(unsigned int ambientIntensityLocation, unsigned int ambientColourLocation,
		unsigned int diffuseIntensityLocation, unsigned int directionLocation);

private:
	glm::vec3 direction;

};

