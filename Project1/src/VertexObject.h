#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

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
	std::vector<unsigned int> shaderRefs;
	bool indexVertices = false;

	unsigned int modelCount;
	std::vector<glm::mat4> models;
public:
	VertexObject(const std::string &path);
	VertexObject(float data[], int size);

	unsigned int getId();
	void init(float data[], int size);
	void addIndices(unsigned int data[], unsigned int size);
	bool genIndices(float data[], int size);
	unsigned int addAttributes(unsigned int size, unsigned int type, bool enable);
	void enableAttribs();
	void drawCall();
	void bind();
	void unbind();

	// shader management related to current object
	void addShader(unsigned int shaderID); // this shader will be compiled and linked before being added as reference
	
	// multiple similar object management
	void addModel(glm::mat4 model = glm::mat4(1.0f)) { models.push_back(model); };
	void updateModel(unsigned int index, float angle, glm::vec3 direction);
	void updateModel(unsigned int index, glm::vec3 translate);
	void updateModel(unsigned int index, float scale);
	void updateModel(unsigned int index, float scalex, float scaley, float scalez);
};
