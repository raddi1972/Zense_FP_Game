#pragma once
#include "VertexObject.h"
class Light :
    public VertexObject
{
private:
    // every light object has a unique id at runtime just like VertexObject

    // basic light properties of the light
    glm::vec3 lightPos;
    glm::vec3 lightColor;

    // lights most prolly wont have specular maps for now
    glm::vec3 ambient, diffuse, specular;
    
    // light ambience properties
    float constant, linear, quadratic;

    void drawCall();
};

