#pragma once

#include <iostream>
#include <map>
#include <utility>
#include <memory>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	float x, y, z;
};

struct TexCoord
{
	float s, t;
};

struct Normal
{
	float x, y, z;
};

struct VertexData
{
	Vertex vertex;
	TexCoord texCoord;
	Normal normal;
};

class VertexObject
{
private:
	unsigned int VAO, VBO, EBO = 0;
	unsigned int vertices = 0;
	unsigned int indices = 0;
	bool indexVertices = false;

	std::unique_ptr<Texture> diffMap;
	std::unique_ptr<Texture> specularMap;

	// for rotation on axis
protected:
	unsigned int shaderIndex;
	glm::vec3 position;
	std::vector<std::pair<float, glm::vec3>> rotations;
	float scale;
	// can read vertices and indices, 
	// For index it will expect a file named path - extension + "_ind" + extension
	// it will return the data in form of pair with first being a pointer to vertex data and
	// second being pointer to index data
	static std::pair<VertexData*, unsigned int*> readData(const std::string& path, unsigned int& vertexCount, unsigned int& indexCount);
	
public:
	std::string name;

	VertexObject(const std::string &path, std::string name);

	unsigned int getId();
	void init(VertexData data[], int size);
	void addIndices(unsigned int data[], unsigned int size);
	void drawCall(std::vector<Shader>& shaders, glm::mat4& view, glm::mat4& projection, glm::vec3 viewPos);
	void bind();
	void unbind();
	void setShader(unsigned int shaderIndex) { this->shaderIndex = shaderIndex; }

	// texture management
	void addMaps(const std::string& diffPath, const std::string& specularPath, bool flipDiff, bool flipSpec);

	void updatePosition(glm::vec3 translate);
	void rotate(float angle, glm::vec3 dir);
	void scaleObject(float scale);
};
