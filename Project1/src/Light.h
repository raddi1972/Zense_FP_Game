#pragma once
#include "VertexObject.h"
class Light :
    public VertexObject
{
public:
    
    // Light contructor
    Light(const std::string& path, unsigned int type, glm::vec3 lightPos, std::string name, float constant, float linear, float quadratic, glm::vec3 lightColor = glm::vec3(1.0f));
    
    // to change the properties of the light
    void setLightProperties(glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f) , glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f));
    
    // to setup the shader's uniforms
    void setUniforms(Shader& shader, unsigned int index);

private:
    // every light object has a unique id at runtime just like VertexObject
    unsigned int type;

    // basic light properties of the light
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    // lights most prolly wont have specular maps for now
    glm::vec3 ambient, diffuse, specular;

    // light ambience properties
    float constant, linear, quadratic;
};

