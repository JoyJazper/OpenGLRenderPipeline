#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

// test flat normal - one normal per triangle and no interpolation - donot use it (performance intense)
// flat in vec3 normal;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

// it takes in the value directly since there is only 1 texture which we are passing which is the active one, setting it in use texture method in texture.cpp
uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	//colour = vCol;

	vec4 specularColour = vec4(0.0f,0.0f,0.0f,0.0f);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		// reflect is in build function which is used to get the mirror vector around second vector.
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f){
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	// texture method is a build it method which takes a texture and uses its coordinate to get the pixel.
	colour = texture(theTexture, texCoord) * (ambientColour + diffuseColour + specularColour);
	//colour = texture(theTexture, texCoord);
}