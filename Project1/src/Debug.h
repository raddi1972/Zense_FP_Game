#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
		return false;
	}
	return true;
}