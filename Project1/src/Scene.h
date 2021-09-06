#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Texture.h"
#include "shader.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
//#include "MovingObject.h"

class Scene
{
public:
	// constructor, take a path to create scene, a near plane and far plane distance to set up projection matrix
	Scene(std::string path, float np = 0.1f, float fp = 100.0f);
	~Scene(); // destructor does not do anything at the moment

	// self explainatory functions, adds various objects to the scene
	void addObject(const std::string& path, std::string name);
	void addModel(const std::string& path);
	void addLight(const std::string& path, unsigned int type, glm::vec3 lightPos, std::string name, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
	unsigned int addShader(const std::string& vs_path, const std::string& fs_path);
	void addTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath, bool flipDiff = true, bool flipSpec= true);
	void addLightTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath);
	
	// update the properties of light, we can change the ambient, diffuse and specular values
	void updateLightPorperties(unsigned int objectIndex, glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f));
	// this function is for changing the position of a object or a light.
	void updateObjectPosition(std::string type, unsigned int objectIndex, glm::vec3 translate);

	// draw call, this function returns a manipulated camera to the caller function
	// this function draws all the objects in the scene based on the properties of the scene and values given to the function.
	// it requires a GLFW window context, and dynamically updated value of width and height of the window for updating the
	// perspective matrix.
	std::shared_ptr<Camera> draw(GLFWwindow* window, float width, float height);

	// a function to map the Shader to either a light or a Object.
	void setShader(std::string type, unsigned int index, unsigned int shaderIndex);

	// getters function for direct manipulation of the object properties, very important for future when we will want to
	// dynamically change the properties of the objects.
	std::vector<VertexObject>& getObjects();
	std::vector<Light>& getLights();

private:
	std::string directory; // stores the directory of scene
	// some folders are expected for the scene to be loaded properly,
	// 1. shaders -> stores all the shaders files
	// 2. textures -> stores all the texture files for object and lights, not for the models as they are stored with the models themselves
	// 3. VertexData -> All the vertex data for the custom objext, first line should specify the number of vertex then we
	//                  we expect the each vertex entry to contain 3 floats for position, 2 for texture mapping and 3 for normal
	//                  coordinates.
	// 4. models -> stores all the data of the models including the textures.

	// self explainatory variables, store the respective data for objects, models, lights, shaders, and a single camera
	std::vector<VertexObject> objects;
	std::vector<Model> models; 
	std::vector<Light> lights; 
	std::vector<Shader> shaders; 
	std::shared_ptr<Camera> camera;

	// values for near plane and far plane taken from constructor
	float p_np, p_fp;
};

