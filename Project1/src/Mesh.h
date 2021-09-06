#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>	
#include <vector>
#include "shader.h"

struct Ver {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

struct Tex {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// mesh data
	std::vector<Ver> vertices;
	std::vector<unsigned int> indices;
	std::vector<Tex> textures;

	Mesh(std::vector<Ver>& vertices, std::vector<unsigned int>& indices, std::vector<Tex>& textures);
	void Draw(Shader& shader);

private:
	// render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

