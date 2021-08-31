#include "VertexObject.h"
#include "Debug.h"

std::pair<VertexData*, unsigned int*> VertexObject::readData(const std::string& path, unsigned int& vertexCount, unsigned int& indexCount)
{
	// setting up the path variables
	std::string vertexPath = path;
	int dotLocation = path.find('.');
	std::string indexPath = path.substr(0, dotLocation) + "_indices" + path.substr(dotLocation);

	// getting the VertexData
	std::ifstream vertexData(vertexPath);
	if (vertexData.fail())
	{
		std::cout << vertexPath << " failed to read the vertex data" << std::endl;
		vertexCount = 0;
		indexCount = 0;
		return std::pair<VertexData*, unsigned int*>(nullptr, nullptr);
	}
	std::stringstream ss;
	std::string s;

	while (std::getline(vertexData, s))
	{
		ss << s << std::endl;
	}
	vertexData.close();
	unsigned int noOfVertices;
	ss >> noOfVertices;

	vertexCount = noOfVertices;

	VertexData *verData = new VertexData[noOfVertices];
	for (int i = 0; i < noOfVertices; i++)
	{
		VertexData f;
		ss >> f.vertex.x;
		ss >> f.vertex.y;
		ss >> f.vertex.z;
		ss >> f.texCoord.s;
		ss >> f.texCoord.t;
		ss >> f.normal.x;
		ss >> f.normal.y;
		ss >> f.normal.z;
		verData[i] = f;
	}

	//getting index data
	std::ifstream indexData(indexPath);
	if (indexData.fail())
	{
		std::cout << indexPath << " Index data not found or error while reading" << std::endl;
		indexCount = 0;
		return std::pair<VertexData*, unsigned int*>(verData, nullptr);
	}
	ss.clear();

	while (std::getline(indexData, s))
	{
		ss << s << std::endl;
	}
	indexData.close();
	unsigned int size;
	ss >> size;
	indexCount = size;

	unsigned int *indData = new unsigned int[size];
	for (int i = 0; i < size; i++)
	{
		unsigned int index;
		ss >> index;
		indData[i] = index;
	}
	return std::pair<VertexData*, unsigned int*>(verData, indData);
}

VertexObject::VertexObject(const std::string& path, std::string name)
	: VAO(0), VBO(0), shaderIndex(0), position(0.0f), name(name), scale(1.0f)
{
	// this constructor reads the vertex data from a file
	auto pair = readData(path, vertices, indices);
	if (!pair.first)
	{
		std::cout << "Error Creating object" << std::endl;
		return;
	}
	init(pair.first, vertices);
	if (pair.second)
	{
		addIndices(pair.second, indices);
		indexVertices = true;
	}
	if(pair.first)
		delete[] pair.first;
	if(pair.second)
		delete[] pair.second;
}



unsigned int VertexObject::getId()
{
	return VAO;
}

void VertexObject::init(VertexData data[], int size)
{
	// generating the vertex array object and vertex buffer object
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));

	// binding the vertex array and vertex buffers
	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	// giving the data to VBO
	GLCall(glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), data, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));
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
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void VertexObject::drawCall(std::vector<Shader>& shaders, glm::mat4& view, glm::mat4& projection, glm::vec3 viewPos)
{
	bind();
	Shader current = shaders[shaderIndex];
	current.use();
	current.setMat4("view", view);
	current.setMat4("projection", projection);
	current.setVec3("viewPos", viewPos);

	std::string name = "material.";
	std::string diffName = name + "diffuse";
	std::string specularName = name + "specular";
	std::string shininess = name + "shininess";
	current.setInt(diffName, 0);
	current.setInt(specularName, 1);
	current.setFloat(shininess, 128.0);

	if(diffMap)
		diffMap->activeTexture(0);
	if(specularMap)
		specularMap->activeTexture(1);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	for (int i = 0; i < rotations.size(); i++)
	{
		model = glm::rotate(model, glm::radians(rotations[i].first), rotations[i].second);
	}

	model = glm::scale(model, glm::vec3(scale));

	current.setMat4("model", model);
	if (!indexVertices)
	{
		GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices));
	}
	else
	{
		GLCall(glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0));
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

void VertexObject::addMaps(const std::string& diffPath, const std::string& specularPath, bool flipDiff, bool flipSpec)
{
	diffMap = std::make_unique<Texture>(diffPath, flipDiff);
	specularMap = std::make_unique<Texture>(specularPath, flipDiff);
}

void VertexObject::updatePosition(glm::vec3 position)
{
	this->position = position;
}

void VertexObject::rotate(float angle, glm::vec3 dir)
{
	for (int i = 0; i < rotations.size(); i++)
	{
		if (rotations[i].second == dir)
		{
			rotations[i].first = rotations[i].first + angle;
			return;
		}
	}
	rotations.push_back(std::pair<float, glm::vec3>(angle, dir));
}

void VertexObject::scaleObject(float scale)
{
	this->scale *= scale;
}



//bool VertexObject::genIndices(float data[], int size)
//{
//	// make sure that the attribs are already set
//	if (!attribs.size())
//	{
//		std::cout << "No attributes setup to make sense of Vertex Data" << std::endl;
//		return false;
//	}
//
//	// we expect the first attrib to be the vertex positions
//	unsigned int skipSize = 0;
//	for (auto it = attribs.begin(); it != attribs.end(); it++)
//			skipSize += (*it).size;
//	unsigned int vertexDataSize = attribs[0].size;
//	unsigned int noOfVertices = size / (skipSize * sizeof(float));
//	std::vector<glm::vec3> indices;
//	unsigned int ind = 0;
//	float* indexedData = new float[noOfVertices * skipSize];
//	for (int i = 0; i < noOfVertices; i++)
//	{
//		glm::vec3 vector;
//		vector.x = data[skipSize * i];
//		vector.y = data[skipSize * i + 1];
//		vector.z = data[skipSize * i + 2];
//		bool skip = false;
//		for (int i = 0; i < indices.size(); i++)
//		{
//			if (indices[i] == vector)
//			{
//				skip = true;
//				break;
//			}
//		}
//		if (skip)
//			continue;
//		for(int j = 0; j < skipSize; j++)
//			indexedData[indices.size() * skipSize + j] = data[skipSize * i + j];
//		indices.push_back(vector);
//	}
//	unsigned int *vertexData = new unsigned int[noOfVertices];
//	for (int i = 0; i < noOfVertices; i++)
//	{
//		glm::vec3 vector(1.0f);
//		vector.x = data[skipSize * i];
//		vector.y = data[skipSize * i + 1];
//		vector.z = data[skipSize * i + 2];
//		for (int j = 0; j < indices.size(); j++)
//		{
//			if (indices[j] == vector)
//			{
//				vertexData[i] = j;
//				break;
//			}
//		}
//	}
//	addIndices(vertexData, noOfVertices * sizeof(unsigned int));
//	// updating vbo
//	GLCall(glBindVertexArray(VAO));
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
//	GLCall(glBufferData(GL_ARRAY_BUFFER, indices.size() * skipSize * sizeof(float), indexedData, GL_STATIC_DRAW));
//	return true;
//}