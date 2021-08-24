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
	std::vector<Texture> textures;
	std::vector<Shader> shaders;
	std::unique_ptr<Camera> camera;
	float p_np, p_fp;

public:
	// constructors
	Scene(float np = 0.1f, float fp = 100.0f);

	void addObject(const std::string& path);
	void addTexture(const std::string& path);
	void addShader(const std::string& vs_path, const std::string& fs_path);
	void draw();
};

