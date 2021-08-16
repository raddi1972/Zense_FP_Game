#include "VertexObject.h"

VertexObject::VertexObject(float data[], int size)
{
	// generating the vertex array object and vertex buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// binding the vertex array and vertex buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// giving the data to VBO
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	bufferSize = size;
}

void VertexObject::addIndices(unsigned int data[])
{
	// we bind the VAO again for security reason
	glBindVertexArray(VAO);

	// we set the flag of index array to be true
	indexVertices = true;

	// create and bind the index buffer object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	// now we send the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

unsigned int VertexObject::addAttributes(unsigned int size, unsigned int type)
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
	attribs.push_back(Attrib{ size, type , size * bytes});

	return i;
}

void VertexObject::enableAttribs()
{
	glBindVertexArray(VAO);
	unsigned int stride = 0;
	for (int i = 0; i < attribs.size(); i++)
		stride += attribs[i].bytes;
	
	unsigned int offset = 0;
	for (int i = 0; i < attribs.size(); i++) {
		glVertexAttribPointer(i, attribs[i].size, attribs[i].type, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += attribs[i].bytes;
	}
}

void VertexObject::drawCall()
{
	bind();
	if (!indexVertices)
	{
		int vertexSize = 0;
		for (auto it = attribs.begin(); it != attribs.end(); it++)
			vertexSize += (*it).bytes;
		int vertices = bufferSize / vertexSize;
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}
}

void VertexObject::bind()
{
	glBindVertexArray(VAO);
}

void VertexObject::unbind()
{
	glBindVertexArray(0);
}