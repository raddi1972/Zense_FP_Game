#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
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

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// defined the material of the object
uniform Material material;

// defined the light propertied
uniform Light light;

void main()
{
	// ambient lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

	// specular lighting
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (vec3(texture(material.specular, texCoord)) * spec) * light.specular;

	// calculating the distance diminishion
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));

	FragColor = vec4(attenuation * (ambient + diffuse + specular)	, 1.0);
}