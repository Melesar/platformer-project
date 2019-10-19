#include "Core.h"
#include "Texture.h"
#include "cassert"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Engine::Texture::Texture(const std::string& filename, TextureId id) : _id(id)
{
    int channels;
    unsigned char* data = stbi_load(filename.c_str(), &_width, &_height, &channels, 0);
	if (data == nullptr)
	{
		std::cerr << "Failed to load texture " << filename << std::endl;
		return;
	}

	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);

	setWrapModeInternal(REPEAT);
	setFilteringInternal(LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

Engine::Texture::~Texture()
{
	glDeleteTextures(1, &_textureData);
}

void Engine::Texture::bind() const
{
	assert(_id >= 0 && _id < MAX_TEXTURES);

	glActiveTexture(GL_TEXTURE0 + _id);
	glBindTexture(GL_TEXTURE_2D, _textureData);
}

int Engine::Texture::width() const
{
	return _width;
}

int Engine::Texture::height() const
{
	return _height;
}

void Engine::Texture::setWrapMode(WrapMode wrapMode)
{
	glBindTexture(GL_TEXTURE_2D, _textureData);
	setWrapModeInternal(wrapMode);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Engine::Texture::setFiltering(FilteringMode filtering)
{
	glBindTexture(GL_TEXTURE_2D, _textureData);
	setFilteringInternal(filtering);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Engine::Texture::setWrapModeInternal(WrapMode wrapMode)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void Engine::Texture::setFilteringInternal(FilteringMode filtering)
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
}


Engine::TextureId Engine::Texture::id() const
{
	return _id;
}
