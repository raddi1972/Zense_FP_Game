#include "Mesh.h"
#include "Debug.h"

Mesh::Mesh(std::vector<Ver>& vertices, std::vector<unsigned int>& indices, std::vector<Tex>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	shader.use();
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrive texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
			name = "diffuse";
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
			name = "specular";
		}
		shader.setInt(("materials[" + number + "]." + name).c_str(), i);
		shader.setFloat(("materials[" + number + "].shininess").c_str(), 128.0f);
		GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}
	// draw mesh
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
}

void Mesh::setupMesh()
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Ver), &vertices[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	// vertex positions
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Ver), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	// vertex texture coords
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Ver), (void*)offsetof(Ver, texCoords)));
	GLCall(glEnableVertexAttribArray(1));
	// vertex normals
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Ver), (void*)offsetof(Ver, normal)));
	GLCall(glEnableVertexAttribArray(2));

	GLCall(glBindVertexArray(0));
}
