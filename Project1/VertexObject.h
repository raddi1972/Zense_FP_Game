#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>

struct Attrib
{
	unsigned int size = 0;
	unsigned int type = GL_FLOAT;
	unsigned int bytes = 0;
};

class VertexObject
{
private:
	unsigned int VAO, VBO, EBO = 0;
	unsigned int bufferSize = 0;
	std::vector<Attrib> attribs;
	bool indexVertices = false;
public:
	VertexObject(float data[], int size);
	void addIndices(unsigned int data[]);
	unsigned int addAttributes(unsigned int size, unsigned int type);
	void enableAttribs();
	void drawCall();
	void bind();
	void unbind();
};

