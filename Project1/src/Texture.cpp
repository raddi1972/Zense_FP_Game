#include "Texture.h"
#include "Debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	// generating the texture buffer object
	GLCall(glGenTextures(1, &texture_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

	// setting the axes settings to repeat image and applying texture filtering
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	unsigned int dataType = GL_RGB;
	if (nrChannels == 1)
		dataType = GL_RED;
	else if (nrChannels == 3)
		dataType = GL_RGB;
	else if (nrChannels == 4)
		dataType = GL_RGBA;

	// adding the texture data to the object
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, dataType, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		success = true;
	}
	else
	{
		success = false;
		std::cout << "Failed" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::bind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
}

void Texture::unBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::activeTexture(unsigned int id)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + id));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
}
