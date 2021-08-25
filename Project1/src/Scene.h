#pragma once

#include <vector>
#include <iostream>
#include "VertexObject.h"
#include "Texture.h"
#include "shader.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
private:
	std::vector<VertexObject> objects;
	std::vector<Light> lights;
	std::vector<Shader> objectShaders;
	std::vector<Shader> lightShaders;
	Camera *camera;
	float p_np, p_fp;

public:
	// constructors
	Scene(float np = 0.1f, float fp = 100.0f);

	void addObject(const std::string& path);
	void addLight(const std::string& path, unsigned int type, glm::vec3 lightPos);
	unsigned int addObjectShader(const std::string& vs_path, const std::string& fs_path);
	unsigned int addLightShader(const std::string& vs_path, const std::string& fs_path);
	void addTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath);

	void setLightModel(unsigned int objectIndex, unsigned int shaderIndex);
	void updateLightModel(unsigned int objectIndex, unsigned int modelNo, float angle, glm::vec3 direction);
	void updateLightModel(unsigned int objectIndex, unsigned int modelNo, glm::vec3 translate);
	void updateLightModel(unsigned int objectIndex, unsigned int modelNo, float scale);
	void updateLightModel(unsigned int objectIndex, unsigned int modelNo, float scalex, float scaley, float scalez);

	void setObjectModel(unsigned int objectIndex, unsigned int shaderIndex);
	void updateObjectModel(unsigned int objectIndex, unsigned int modelNo, float angle, glm::vec3 direction);
	void updateObjectModel(unsigned int objectIndex, unsigned int modelNo, glm::vec3 translate);
	void updateObjectModel(unsigned int objectIndex, unsigned int modelNo, float scale);
	void updateObjectModel(unsigned int objectIndex, unsigned int modelNo, float scalex, float scaley, float scalez);

	Camera* draw(GLFWwindow* window, float width, float height);
};

