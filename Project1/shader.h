#pragma once
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
private:
	unsigned int m_ShaderID;
	int m_Success;
	unsigned int getShaders(const char* path, unsigned int type);

public:
	// we take the vertexPath and the fragment path and
	Shader(const char* vertexPath, const char* fragmentPath);
	
	// if we wish to activate this shader
	bool use();
	unsigned int getShaderID();
};