#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture
{
	// texture id
	unsigned int texture_id;
	// flag for success texture load
	bool success;

public:
	// constructor to load textures
	Texture(unsigned char* data, int width, int height, unsigned int dataType);
	Texture(const std::string& path, bool flip = true);

	void bind();
	void unBind();
	void activeTexture(unsigned int id);
};