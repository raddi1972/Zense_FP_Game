#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	float shininess;
};

struct Light {
	vec3 position;

	// adding the values for properties of light
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// adding the values for distance diminishing of light
	float constant;
	float linear;
	float quadratic;
};

// This is the output of fragment shader
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	// calculation for ambient lighting
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

	// calculation for diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * vec3(texture(material.texture_diffuse1, TexCoords))) * light.diffuse;

	// calculation for specular lighting
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (vec3(texture(material.texture_specular1, TexCoords)) * spec) * light.specular;

	// calculating the distance diminishion
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}