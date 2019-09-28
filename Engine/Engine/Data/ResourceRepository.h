﻿#pragma once
#include <Core.h>

namespace Engine
{
	class Shader;
	class Texture;

	enum TextureId
	{
		TEX_ELLIOT,
		TEX_BACKGROUND,
		TEX_ENEMY,
		TEX_PLATFORM,

		NUM_TEXTURES
	};

	enum ShaderType
	{
		SHADER_SPRITE,

		NUM_SHADERS
	};
	
	
	class ENGINE_API ResourcesRepository
	{
	public:
		std::shared_ptr<Shader> getShader(ShaderType type) const;
		std::shared_ptr<Texture> getTexture(TextureId id) const;

		void loadResources();

		ResourcesRepository() = default;
		ResourcesRepository(const ResourcesRepository& other) = delete;

		virtual ~ResourcesRepository() = default;

	private:

		void loadShaders();
		void loadTextures();

		void loadTexture(const std::string& textureName, TextureId id);
		void loadShader(const std::string& shaderName, ShaderType type);
		
	private:

		std::map<TextureId, std::shared_ptr<Texture>> _textures;
		std::map<ShaderType, std::shared_ptr<Shader>> _shaders;

		const std::string texturesFolder = "../res/Textures/";
		const std::string shadersFolder = "../res/Shaders/";
	};

}

