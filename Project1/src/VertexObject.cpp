#include "VertexObject.h"
#include "Debug.h"

VertexObject::VertexObject(const std::string& path)
{
	// this constructor reads the vertex data from a file
	std::ifstream objectData(path);
	if (objectData.fail())
	{
		std::cout << path << " Object data not read properly" << std::endl;
		return;
	}
	std::stringstream ss;
	std::string s;
	
	while (std::getline(objectData, s))
	{
		ss << s << std::endl;
	}
	objectData.close();
	unsigned int noOfVertices, noOfAtributes;
	ss >> noOfVertices;
	ss >> noOfAtributes;

	unsigned int sizeOfVertex = 0;
	for (int i = 0; i < noOfAtributes; i++)
	{
		unsigned int size;
		ss >> size;
		sizeOfVertex += size;
		addAttributes(size, GL_FLOAT, true);
	}
	unsigned int dataSize = sizeOfVertex * noOfVertices;
	
	std::unique_ptr<float[]> data = std::make_unique<float[]>(dataSize);
	for (int i = 0; i < sizeOfVertex * noOfVertices; i++)
	{
		float f;
		ss >> f;
		data[i] = f;
	}
	init(data.get(), dataSize * sizeof(float));
	enableAttribs();
}

VertexObject::VertexObject(float data[], int size)
{
	init(data, size);
}

unsigned int VertexObject::getId()
{
	return VAO;
}

void VertexObject::init(float data[], int size)
{
	// generating the vertex array object and vertex buffer object
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));

	// binding the vertex array and vertex buffers
	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	// giving the data to VBO
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	bufferSize = size;

	modelCount = 0;
}

void VertexObject::addIndices(unsigned int data[], unsigned int size)
{
	// we bind the VAO again for security reason
	GLCall(glBindVertexArray(VAO));

	// we set the flag of index array to be true
	indexVertices = true;

	// create and bind the index buffer object
	GLCall(glGenBuffers(1, &EBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	
	// now we send the data
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

bool VertexObject::genIndices(float data[], int size)
{
	// make sure that the attribs are already set
	if (!attribs.size())
	{
		std::cout << "No attributes setup to make sense of Vertex Data" << std::endl;
		return false;
	}

	// we expect the first attrib to be the vertex positions
	unsigned int skipSize = 0;
	for (auto it = attribs.begin(); it != attribs.end(); it++)
			skipSize += (*it).size;
	unsigned int vertexDataSize = attribs[0].size;
	unsigned int noOfVertices = size / (skipSize * sizeof(float));
	std::vector<glm::vec3> indices;
	unsigned int ind = 0;
	float* indexedData = new float[noOfVertices * skipSize];
	for (int i = 0; i < noOfVertices; i++)
	{
		glm::vec3 vector;
		vector.x = data[skipSize * i];
		vector.y = data[skipSize * i + 1];
		vector.z = data[skipSize * i + 2];
		bool skip = false;
		for (int i = 0; i < indices.size(); i++)
		{
			if (indices[i] == vector)
			{
				skip = true;
				break;
			}
		}
		if (skip)
			continue;
		for(int j = 0; j < skipSize; j++)
			indexedData[indices.size() * skipSize + j] = data[skipSize * i + j];
		indices.push_back(vector);
	}
	unsigned int *vertexData = new unsigned int[noOfVertices];
	for (int i = 0; i < noOfVertices; i++)
	{
		glm::vec3 vector(1.0f);
		vector.x = data[skipSize * i];
		vector.y = data[skipSize * i + 1];
		vector.z = data[skipSize * i + 2];
		for (int j = 0; j < indices.size(); j++)
		{
			if (indices[j] == vector)
			{
				vertexData[i] = j;
				break;
			}
		}
	}
	addIndices(vertexData, noOfVertices * sizeof(unsigned int));
	// updating vbo
	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, indices.size() * skipSize * sizeof(float), indexedData, GL_STATIC_DRAW));
	return true;
}

unsigned int VertexObject::addAttributes(unsigned int size, unsigned int type, bool enable)
{
	int i = attribs.size();
	unsigned int bytes = 0;
	switch (type)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		bytes = sizeof(char);
		break;
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		bytes = sizeof(short);
		break;
	case GL_INT:
	case GL_UNSIGNED_INT:
		bytes = sizeof(int);
		break;
	default:
		bytes= sizeof(float);
		break;
	}
	attribs.push_back(Attrib{ size, type , size * bytes, enable});

	return i;
}

void VertexObject::enableAttribs()
{
	GLCall(glBindVertexArray(VAO));
	unsigned int stride = 0;
	for (int i = 0; i < attribs.size(); i++)
		stride += attribs[i].bytes;
	
	unsigned int offset = 0;
	for (int i = 0; i < attribs.size(); i++) {
		if (attribs[i].toEnable) {
			GLCall(glVertexAttribPointer(i, attribs[i].size, attribs[i].type, GL_FALSE, stride, (void*)offset));
			GLCall(glEnableVertexAttribArray(i));
		}
		offset += attribs[i].bytes;
	}
}

void VertexObject::drawCall(std::vector<Shader>& shaders, glm::mat4& view, glm::mat4& projection, glm::vec3 viewPos)
{
	int vertexSize = 0;
	for (auto it = attribs.begin(); it != attribs.end(); it++)
		vertexSize += (*it).bytes;
	int vertices = bufferSize / vertexSize;
	bind();
	for (int i = 0; i < models.size(); i++)
	{
		Shader current = shaders[modelShader[i]];
		current.use();
		current.setMat4("model", models[i]);
		current.setMat4("view", view);
		current.setMat4("projection", projection);
		current.setVec3("viewPos", viewPos);

		for (int j = 0; j < diffMaps.size(); j++)
		{
			std::string name = "material.";
			std::string diffName = name + "diffuse";
			std::string specularName = name + "specular";
			std::string shininess = name + "shininess";
			current.setInt(diffName, 0);
			current.setInt(specularName, 1);
			current.setFloat(shininess, 64.0);
			diffMaps[j].activeTexture(0);
			specularMaps[j].activeTexture(1);
		}
		
		if (!indexVertices)
		{
			GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices));
		}
		else
		{
			GLCall(glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0));
		}
	}
}

void VertexObject::bind()
{
	GLCall(glBindVertexArray(VAO));
}

void VertexObject::unbind()
{
	GLCall(glBindVertexArray(0));
}

void VertexObject::addMaps(const std::string& diffPath, const std::string& specularPath)
{
	diffMaps.push_back(Texture(diffPath));
	specularMaps.push_back(Texture(specularPath));
}

void VertexObject::updateModel(unsigned int index, float angle, glm::vec3 direction)
{
	models[index] = glm::rotate(models[index], angle, direction);
}

void VertexObject::updateModel(unsigned int index, glm::vec3 translate)
{
	models[index] = glm::translate(models[index], translate);
}

void VertexObject::updateModel(unsigned int index, float scale)
{
	models[index] = glm::scale(models[index], glm::vec3(scale));
}

void VertexObject::updateModel(unsigned int index, float scalex, float scaley, float scalez)
{
	models[index] = glm::scale(models[index], glm::vec3(scalex, scaley, scalez));
}
