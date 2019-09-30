#include "Application.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"

Platformer::Application::Application()
{
	_title = "Platformer project";
	_isFullscreen = true;
	_worldHeight = 7.5f;
}

void Platformer::Application::setup()
{
	Engine::Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	Engine::Sprite* playerSprite = createSprite(Engine::TEX_ELLIOT, 500);
	_player = std::make_unique<Player>(playerSprite, _input, _raycaster);

	createPlatform({0, -4}, {_renderer->worldWidth(), 1});
	createPlatform({ 3, -2 });
}

bool flip = false;

void Platformer::Application::update(float deltaTime)
{
	_player->update(deltaTime);
}

void Platformer::Application::onExit()
{
	Engine::Application::onExit();
}

Engine::Sprite* Platformer::Application::createPlatform(glm::vec2 position)
{
	Engine::Sprite* platform = createSprite(Engine::TEX_PLATFORM, 256);
	platform->setLayer(Engine::BoundingBox::PLATFORM);
	platform->setPosition(position);
	platform->setSortingOrder(10);

	return platform;
}

Engine::Sprite* Platformer::Application::createPlatform(glm::vec2 position, glm::vec2 size)
{
	Engine::Sprite* platform = createPlatform(position);
	platform->setSize(size);

	return platform;
}
