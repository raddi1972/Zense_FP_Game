#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Texture.h"
#include "shader.h"
#include "Camera.h"
#include "Light.h"
//#include "MovingObject.h"

class Scene
{
private:
	std::string directory;
	std::vector<VertexObject> objects;
	std::vector<Light> lights;
	std::vector<Shader> shaders;
	std::shared_ptr<Camera> camera;
	float p_np, p_fp;

public:
	// constructors
	Scene(std::string path, float np = 0.1f, float fp = 100.0f);
	~Scene();

	void addObject(const std::string& path, std::string name);
	void addLight(const std::string& path, unsigned int type, glm::vec3 lightPos, std::string name);
	unsigned int addShader(const std::string& vs_path, const std::string& fs_path);
	void addTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath, bool flipDiff = true, bool flipSpec= true);

	void addLightTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath);
	void updateLightPorperties(unsigned int objectIndex, glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f));

	void updateObjectPosition(std::string type, unsigned int objectIndex, glm::vec3 translate);

	std::shared_ptr<Camera> draw(GLFWwindow* window, float width, float height);
	void setShader(std::string type, unsigned int index, unsigned int shaderIndex);

	std::vector<VertexObject>& getObjects();
	std::vector<Light>& getLights();
};

