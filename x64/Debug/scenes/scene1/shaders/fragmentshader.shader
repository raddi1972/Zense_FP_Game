#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
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

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform int lightNo;

// defined the material of the object

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// normal and vieDir
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	// directional light
	//CalcDirLight(dirLight, norm, viewDir);
	vec3 result = vec3(0.0);
	// point lights
	for(int i = 0; i < lightNo; i++)
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

// a function to calculate directional light
// Note: directional lights do not have attenuation as they are not dependent
// on distance
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	// vector need to go from fragment to outward direction
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	// reflect function calculat I - 2.0 * dot(I, N) * N
	vec3 refDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, refDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// calculating light Direction as light does not have direction here
	vec3 lightDir = normalize(light.position - fragPos); // negative of lightDirection

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 refDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, refDir), 0.0), material.shininess);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
	return attenuation * (ambient + diffuse + specular);
}