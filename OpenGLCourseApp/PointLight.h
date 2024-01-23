#pragma once
#include "Light.h"
class PointLight :
    public Light
{


private:
    glm::vec3 position;
    GLfloat constant, linear, exponent;

};

