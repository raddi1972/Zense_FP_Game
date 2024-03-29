#include "Scene.h"
//#include "Debug.h"

// constructors implementation
Scene::Scene(std::string path, float np, float fp) : directory(path)
{
	// p_np -> perspective near plane
	// p_fp -> perspective far plane
	p_np = np; p_fp = fp;

	// setting up the camera for view matrix
	// only 1 camera per scene
	camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

Scene::~Scene()
{
	std::cout << "Object Deleted" << std::endl; // just to notify if we the object is deleted
}

void Scene::addObject(const std::string& path, std::string name)
{
	std::string newPath = directory + "/VertexData/" + path;
	objects.push_back(VertexObject(newPath, name));
}

void Scene::addModel(const std::string& path)
{
	std::string newPath = directory + "/models/" + path;
	models.push_back(Model(newPath));
}

void Scene::addLight(const std::string& path, unsigned int type, glm::vec3 lightPos, std::string name, float constant, float linear, float quadratic)
{
	std::string newPath = directory + "/VertexData/" + path;
	lights.push_back(Light(newPath, type, lightPos, name, constant, linear, quadratic));
}

unsigned int Scene::addShader(const std::string& vs_path, const std::string& fs_path)
{
	std::string vs_newPath = directory + "/shaders/" + vs_path;
	std::string fs_newPath = directory + "/shaders/" + fs_path;
	shaders.push_back(Shader(vs_newPath.c_str(), fs_newPath.c_str()));
	return (shaders.size() - 1);
}

std::shared_ptr<Camera> Scene::draw(GLFWwindow* window, float width, float height)
{

	// setting up view matrix based on the camera properties
	glm::mat4 view = camera->getView();

	// setting up projection matrix, rquires camera->getZoom() gets the fov that gives, we also require aspect ratio
	// i.e. width/height, then we also have near plane and far plane
	glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), width / height, p_np, p_fp);

	// setting up shaders and sending the uniforms of lights to shaders
	for (int i = 0; i < shaders.size(); i++)
	{
		shaders[i].use();
		shaders[i].setInt("lightNo", lights.size());
	}
	for (int i = 0; i < shaders.size(); i++)
	{
		for (int j = 0; j < lights.size(); j++)
			lights[j].setUniforms(shaders[i], j);
	}

	// drawcall for objects, lights and models
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].drawCall(shaders, view, projection, camera->getPosition());
	}

	for (int i = 0; i < lights.size(); i++)
		lights[i].drawCall(shaders, view, projection, camera->getPosition());

	Shader current = shaders[0];
	current.use();
	current.setMat4("view", view);
	current.setMat4("projection", projection);
	current.setMat4("model", glm::mat4(1.0f));
	current.setVec3("viewPos", camera->getPosition());
	for (int i = 0; i < models.size(); i++)
	{
		models[i].Draw(shaders[0]);
	}

	// returning the camera
	return camera;
}

void Scene::setShader(std::string type, unsigned int index, unsigned int shaderIndex)
{
	if (shaderIndex >= shaders.size())
	{
		std::cout << "This shader index does not exists" << std::endl;
		return;
	}
	if (type == "light")
	{
		if (index >= lights.size())
		{
			std::cout << "This index of light does not exists." << std::endl;
			return;
		}
		lights[index].setShader(shaderIndex);
	}
	else if (type == "object")
	{
		if (index >= objects.size())
		{
			std::cout << "This index of object does not exists." << std::endl;
			return;
		}
		objects[index].setShader(shaderIndex);
	}
	else {
		std::cout << "Invalid type of object" << std::endl;
	}
}

std::vector<VertexObject>& Scene::getObjects()
{
	// TODO: insert return statement here
	return objects;
}

std::vector<Light>& Scene::getLights()
{
	// TODO: insert return statement here
	return lights;
}

void Scene::addTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath, bool flipDiff, bool flipSpec)
{
	std::string diff_newPath = directory + "/textures/" + diffPath;
	std::string spec_newPath = directory + "/textures/" + specularPath;
	objects[objectIndex].addMaps(diff_newPath, spec_newPath, flipDiff, flipSpec);
}

void Scene::addLightTexture(unsigned int objectIndex, const std::string& diffPath, const std::string& specularPath)
{
	std::string diff_newPath = directory + "/textures/" + diffPath;
	std::string spec_newPath = directory + "/textures/" + specularPath;
	lights[objectIndex].addMaps(diff_newPath, spec_newPath, true, true);
}

void Scene::updateLightPorperties(unsigned int objectIndex, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	lights[objectIndex].setLightProperties(ambient, diffuse, specular);
}

void Scene::updateObjectPosition(std::string type, unsigned int objectIndex, glm::vec3 translate)
{
	if (objectIndex >= objects.size())
	{
		std::cout << "This shader index does not exists" << std::endl;
		return;
	}
	if (type == "light")
	{
		if (objectIndex >= lights.size())
		{
			std::cout << "This index of light does not exists." << std::endl;
			return;
		}
		lights[objectIndex].updatePosition(translate);
	}
	else if (type == "object")
	{
		if (objectIndex >= objects.size())
		{
			std::cout << "This index of object does not exists." << std::endl;
			return;
		}
		objects[objectIndex].updatePosition(translate);
	}
	else {
		std::cout << "Invalid type of object" << std::endl;
	}
}


