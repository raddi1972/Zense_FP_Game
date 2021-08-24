#include "Scene.h"

// constructors implementation
Scene::Scene(float np, float fp)
{
	// p_np -> perspective near plane
	// p_fp -> perspective far plane
	p_np = np; p_fp = fp;

	// setting up the camera for view matrix
	// only 1 camera per scene
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Scene::addObject(const std::string& path)
{
	objects.push_back(VertexObject(path));
}

void Scene::addTexture(const std::string& path)
{
	textures.push_back(Texture(path));
}

void Scene::addShader(const std::string& vs_path, const std::string& fs_path)
{
	shaders.push_back(Shader(vs_path.c_str(), fs_path.c_str()));
}

void Scene::draw()
{
	
}