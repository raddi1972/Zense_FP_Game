#pragma once
#include "VertexObject.h"
class Light :
    public VertexObject
{
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
public:
    // constructors
    Light(const std::string& path, unsigned int type, glm::vec3 lightPos, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f, glm::vec3 lightColor = glm::vec3(1.0f));
    void setLightProperties(glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f) , glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f));
    void drawCall(std::vector<Shader>& shaders, glm::mat4& view, glm::mat4& projection, glm::vec3 viewDir);
    void setUniforms(Shader& shader, unsigned int index);
};

