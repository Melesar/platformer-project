#include "Core.h"
#include "Texture.h"
#include "cassert"

Engine::Texture::Texture(const std::string& filename, TextureId id) : _id(id)
{
	SDL_Surface* imageData = IMG_Load(filename.c_str());
	if (imageData == nullptr)
	{
		std::cerr << "Failed to load texture " << filename << std::endl;
		return;
	}

	_width = imageData->w;
	_height = imageData->h;

	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData->w, imageData->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(imageData);
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

Engine::TextureId Engine::Texture::id() const
{
	return _id;
}
