#pragma once

#include <iostream>
#include <map>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Attrib
{
	unsigned int size = 0;
	unsigned int type = GL_FLOAT;
	unsigned int bytes = 0;
	bool toEnable;
};

class VertexObject
{
private:
	unsigned int VAO, VBO, EBO = 0;
	unsigned int bufferSize = 0;
	std::vector<Attrib> attribs;
	bool indexVertices = false;

	unsigned int modelCount;
	std::vector<glm::mat4> models;
	std::map<unsigned int, unsigned int> modelShader;
	std::vector<Texture> diffMaps;
	std::vector<Texture> specularMaps;
	
public:
	VertexObject(const std::string &path);
	VertexObject(float data[], int size);

	unsigned int getId();
	void init(float data[], int size);
	void addIndices(unsigned int data[], unsigned int size);
	bool genIndices(float data[], int size);
	unsigned int addAttributes(unsigned int size, unsigned int type, bool enable);
	void enableAttribs();
	void drawCall(std::vector<Shader>& shaders, glm::mat4& view, glm::mat4& projection, glm::vec3 viewPos);
	void bind();
	void unbind();

	// texture management
	void addMaps(const std::string& diffPath, const std::string& specularPath);

	// multiple similar object management
	void addModel(unsigned int shaderIndex, glm::mat4 model = glm::mat4(1.0f)) { models.push_back(model); modelShader[models.size() - 1] = shaderIndex; };
	void updateModel(unsigned int index, float angle, glm::vec3 direction);
	void updateModel(unsigned int index, glm::vec3 translate);
	void updateModel(unsigned int index, float scale);
	void updateModel(unsigned int index, float scalex, float scaley, float scalez);
};
