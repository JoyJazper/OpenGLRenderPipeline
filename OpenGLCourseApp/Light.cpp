#include "Light.h"

Light::Light() 
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
{
	// set values for a variable in the shader. 2 arguments are ID's of the 2 variables.
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::~Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 0;
}