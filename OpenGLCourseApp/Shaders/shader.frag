#version 330

in vec4 vCol;
in vec2 texCoord;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
};

// it takes in the value directly since there is only 1 texture which we are passing which is the active one, setting it in use texture method in texture.cpp
uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	//colour = vCol;
	// texture method is a build it method which takes a texture and uses its coordinate to get the pixel.
	colour = texture(theTexture, texCoord) * ambientColour;
}