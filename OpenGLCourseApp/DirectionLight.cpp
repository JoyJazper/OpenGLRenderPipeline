#include "DirectionLight.h"

DirectionLight::DirectionLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

/*Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
}*/

DirectionLight::DirectionLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

/*void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
{
	// set values for a variable in the shader. 2 arguments are ID's of the 2 variables.
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}*/

void DirectionLight::UseDirectionLight(unsigned int ambientIntensityLocation, unsigned int ambientColourLocation,
	unsigned int diffuseIntensityLocation, unsigned int directionLocation)
{
	// set values for a variable in the shader. 2 arguments are ID's of the 2 variables.
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionLight::~DirectionLight() 
{

}
