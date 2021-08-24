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

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ShaderID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}