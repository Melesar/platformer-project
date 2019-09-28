#pragma once
#include "Data/ResourceRepository.h"

namespace Engine
{
	
	class ENGINE_API Texture
	{
	public:

		Texture(const std::string& filename, TextureId id);
		Texture(const Texture& other) = delete;
		
		~Texture();

		void bind() const;

		int width() const;
		int height() const;

		TextureId id() const;
		
	private:

		const int MAX_TEXTURES = 32;

		int _width, _height;
		TextureId _id;
		
		GLuint _textureData;
	};

}
