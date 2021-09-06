#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

out vec4 FragColor;

in vec2 texCoord;

void main()
{
	FragColor = texture(material.diffuse, texCoord);
}