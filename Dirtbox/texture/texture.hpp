#pragma once
#include <GL/glew.h>
#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "../stbi/stb_image.h"

#include "../debug/debug.hpp"


class Texture
{
private:
	unsigned int _id;
	int _width, _height;
	bool _alpha;
	
public:
	Texture()
	: _id(0)
	{
		
	}

	Texture(unsigned int id, int width, int height, bool alpha)
	: _id(id), _width(width), _height(height), _alpha(alpha)
	{
		
	}
	
	static Texture loadFromFile(std::string path)
	{
		unsigned int id;
		int width, height, channels;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if(data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, (channels == 3)? GL_RGB : GL_RGBA, width, height, 0, (channels == 3)? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else
		{
			#ifdef DEBUG
			std::cout << "ERROR: Failed to load texture \"" << path << "\"" << std::endl;
			#endif 
			stbi_image_free(data);
			return Texture();
		}

		return Texture(id, width, height, (channels == 3)? false : true);
	}

	void use(unsigned int id)
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	unsigned int getId() const
	{
		return _id;
	}
	
	unsigned int getWidth() const
	{
		return _width;
	}
	
	unsigned int getHeight() const
	{
		return _height;
	}
	
	operator bool() const
	{
		return _id;
	}
};
