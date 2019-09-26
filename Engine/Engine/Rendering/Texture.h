#pragma once

namespace Engine
{
	enum TextureId
	{
		NONE,
		ELLIOT,
	};
	
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
		TextureId _id = NONE;
		
		GLuint _textureData;
	};

}
