#include "Light.h"

Light::Light(const std::string& path, unsigned int type, glm::vec3 lightPos, std::string name, float constant, float linear, float quadratic, glm::vec3 lightColor) : VertexObject::VertexObject(path, name)
{
	// setting up the starter values
	shaderIndex = 1;
	this->type = type;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->lightColor = lightColor;
	this->lightPos = lightPos;
	this->updatePosition(lightPos);
	setLightProperties();
}

void Light::setLightProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::setUniforms(Shader& shader, unsigned int index)
{
	// index gives the light index
	shader.use();
	std::string prefix;
	if (type == 0) // point light
	{
		prefix = "pointLights[" + std::to_string(index) + "]";
	}
	else if (type == 1) // directional light
	{
		prefix = "dirLight";
	}
	shader.setFloat((prefix + ".constant").c_str(), constant);
	shader.setFloat((prefix + ".linear").c_str(), linear);
	shader.setFloat((prefix + ".quadratic").c_str(), quadratic);


	shader.setVec3((prefix + ".position").c_str(), lightPos);
	shader.setVec3((prefix + ".ambient").c_str(), ambient);
	shader.setVec3((prefix + ".diffuse").c_str(), diffuse);
	shader.setVec3((prefix + ".specular").c_str(), specular);

}
