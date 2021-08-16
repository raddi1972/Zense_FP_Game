#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	int vertexShader = getShaders(vertexPath, GL_VERTEX_SHADER);
	int fragmentSader = getShaders(fragmentPath, GL_FRAGMENT_SHADER);
	m_ShaderID = glCreateProgram();
	glAttachShader(m_ShaderID, vertexShader);
	glAttachShader(m_ShaderID, fragmentSader);
	glLinkProgram(m_ShaderID);

	char infoLog[512]; // tells the log status i.e. what went wrong.
	glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &m_Success);
	if (!m_Success)
	{
		glGetProgramInfoLog(m_ShaderID, 512, NULL, infoLog);
		std::cout << "Shader program compilation failed\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentSader);
}

unsigned int Shader::getShaders(const char* path, unsigned int type)
{
	// reading the given shader file
	std::ifstream shader(path);
	if (shader.fail())
	{
		std::cout << path << " shader was not read successfully." << std::endl;
		return 0;
	}
	std::string s;
	std::stringstream ss;
	while (std::getline(shader, s))
	{
		ss << s << std::endl;
	}
	shader.close();

	// we create a shader object
	unsigned int shaderID = glCreateShader(type);
	const std::string temp = ss.str();
	const char* code = temp.c_str();

	std::cout << code << std::endl;

	// attach the source code to the shader
	glShaderSource(shaderID, 1, &code, NULL);
	glCompileShader(shaderID); // compiles the shader

	// error checking for the shaders
	int success; // tells if the compilation was success or not
	char infoLog[512]; // tells the log status i.e. what went wrong.
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << path << " shader compilation failed\n" << infoLog << std::endl;
	}
	return shaderID;
}

bool Shader::use()
{
	if (m_Success)
	{
		glUseProgram(m_ShaderID);
		return true;
	}
	return false;
}

unsigned int Shader::getShaderID()
{
	return m_ShaderID;
}