#include "Scene.h"
//#include "Debug.h"

// constructors implementation
Scene::Scene(float np, float fp)
{
	// p_np -> perspective near plane
	// p_fp -> perspective far plane
	p_np = np; p_fp = fp;

	// setting up the camera for view matrix
	// only 1 camera per scene
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Scene::addObject(const std::string& path)
{
	objects.push_back(VertexObject(path));
}

void Scene::addLight(const std::string& path, unsigned int type, glm::vec3 lightPos)
{
	lights.push_back(Light(path, type, lightPos));
}

unsigned int Scene::addObjectShader(const std::string& vs_path, const std::string& fs_path)
{
	objectShaders.push_back(Shader(vs_path.c_str(), fs_path.c_str()));
	return (objectShaders.size() - 1);
}

Camera* Scene::draw(GLFWwindow* window, float width, float height)
{
	glm::mat4 view = camera->getView();
	glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), width / height, p_np, p_fp);

	for (int i = 0; i < objectShaders.size(); i++)
	{
		for (int j = 0; j < lights.size(); j++)
			lights[j].setUniforms(objectShaders[i], j);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].drawCall(objectShaders, view, projection, camera->getPosition());
	}

	for (int i = 0; i < lights.size(); i++)
		lights[i].drawCall(lightShaders, view, projection, camera->getPosition());
	return camera;
}

void Scene::addTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath)
{
	objects[objectIndex].addMaps(diffPath, specularPath);
}

unsigned int Scene::addLightShader(const std::string& vs_path, const std::string& fs_path)
{
	lightShaders.push_back(Shader(vs_path.c_str(), fs_path.c_str()));
	return (lightShaders.size() - 1);
}

void Scene::setLightModel(unsigned int lightIndex, unsigned int shaderIndex)
{
	lights[lightIndex].addModel(shaderIndex);
}

void Scene::updateLightModel(unsigned int lightIndex, unsigned int modelNo, float angle, glm::vec3 direction)
{
	lights[lightIndex].updateModel(modelNo, angle, direction);
}

void Scene::updateLightModel(unsigned int lightIndex, unsigned int modelNo, glm::vec3 translate)
{
	lights[lightIndex].updateModel(modelNo, translate);
}

void Scene::updateLightModel(unsigned int lightIndex, unsigned int modelNo, float scale)
{
	lights[lightIndex].updateModel(modelNo, scale);
}

void Scene::updateLightModel(unsigned int lightIndex, unsigned int modelNo, float scalex, float scaley, float scalez)
{
	lights[lightIndex].updateModel(modelNo, scalex, scaley, scalez);
}

void Scene::setObjectModel(unsigned int objectIndex, unsigned int shaderIndex)
{
	objects[objectIndex].addModel(shaderIndex);
}

void Scene::updateObjectModel(unsigned int objectIndex, unsigned int modelNo, float angle, glm::vec3 direction)
{
	objects[objectIndex].updateModel(modelNo, angle, direction);
}

void Scene::updateObjectModel(unsigned int objectIndex, unsigned int modelNo, glm::vec3 translate)
{
	objects[objectIndex].updateModel(modelNo, translate);
}

void Scene::updateObjectModel(unsigned int objectIndex, unsigned int modelNo, float scale)
{
	objects[objectIndex].updateModel(modelNo, scale);
}

void Scene::updateObjectModel(unsigned int objectIndex, unsigned int modelNo, float scalex, float scaley, float scalez)
{
	objects[objectIndex].updateModel(modelNo, scalex, scaley, scalez);
}
