#pragma once
#include <Core.h>
#include <Enums.h>


namespace Engine
{
	enum WrapMode
	{
		REPEAT = GL_REPEAT,
		CLAMP = GL_CLAMP,
		BORDER = GL_CLAMP_TO_BORDER
	};

	enum FilteringMode
	{
		POINT = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR = GL_LINEAR_MIPMAP_LINEAR,
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

		void setWrapMode(WrapMode wrapMode);
		void setFiltering(FilteringMode filtering);

		TextureId id() const;

	private:

		static void setWrapModeInternal(WrapMode wrapMode);
		static void setFilteringInternal(FilteringMode filtering);
		
	private:

		const int MAX_TEXTURES = 32;

		int _width, _height;
		TextureId _id;
		
		GLuint _textureData;
	};

}
