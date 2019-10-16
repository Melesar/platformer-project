#include "Core.h"
#include "ResourceRepository.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"

std::shared_ptr<Engine::Shader> Engine::ResourcesRepository::getShader(ShaderType type) const
{
	auto iter = _shaders.find(type);
	return iter != _shaders.end() ? iter->second : nullptr;
}

std::shared_ptr<Engine::Texture> Engine::ResourcesRepository::getTexture(TextureId id) const
{
	auto iter = _textures.find(id);
	return iter != _textures.end() ? iter->second : nullptr;
}

void Engine::ResourcesRepository::loadResources()
{
	loadShaders();
	loadTextures();
}

void Engine::ResourcesRepository::loadShaders()
{
	loadShader("sprites", SHADER_SPRITE);
}

void Engine::ResourcesRepository::loadTextures()
{
	loadTexture("Platform.png", TEX_PLATFORM);
	loadTexture("Elliot.png", TEX_ENEMY);
	loadTexture("Player.png", TEX_PLAYER)->setWrapMode(BORDER);
	loadTexture("Bullet.png", TEX_BULLET);
	loadTexture("Heart.png", TEX_HEART)->setWrapMode(BORDER);
	loadTexture("Heart_empty.png", TEX_HEART_EMPTY)->setWrapMode(BORDER);
	loadTexture("Restart_button.png", TEX_BUTTON_RESTART);
	loadTexture("Game_over.png", TEX_GAME_OVER);
}

std::shared_ptr<Engine::Texture> Engine::ResourcesRepository::loadTexture(const std::string& textureName, TextureId id)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(texturesFolder + textureName, id);
	_textures.emplace(std::pair<TextureId, std::shared_ptr<Texture>>(id, texture));

	return texture;
}

void Engine::ResourcesRepository::loadShader(const std::string& shaderName, ShaderType type)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(shadersFolder + shaderName);
	_shaders.emplace(std::pair<ShaderType, std::shared_ptr<Shader>>(type, shader));
}

