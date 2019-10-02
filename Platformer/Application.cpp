#include "Application.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"

Platformer::Application::Application()
{
	_title = "Platformer project";
	_worldHeight = 10.f;
}

void Platformer::Application::setup()
{
	Engine::Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	Engine::Sprite* playerSprite = createSprite(Engine::TEX_ELLIOT, 512);
	_player = std::make_unique<Player>(playerSprite, _input, _raycaster);
	
	glm::vec2 worldSize = { _renderer->worldWidth(), _renderer->worldHeight() };
	createPlatform({0, -4.5f}, { worldSize.x, 1});
	createPlatform({ 3, 0 });
	createPlatform({ -3, -2 }, { 6, 1 });

	Engine::BoundingBox leftWall{ {-0.5f * worldSize.x - 1.f, -0.5f * worldSize.y}, {-0.5f * worldSize.x, 0.5f * worldSize.y} };
	Engine::BoundingBox rightWall{ {0.5f * worldSize.x, -0.5f * worldSize.y}, {0.5f * worldSize.x + 1.f, 0.5f * worldSize.y} };
	Engine::BoundingBox topWall{ {-0.5f * worldSize.x, 0.5f * worldSize.y}, {0.5f * worldSize.x, 0.5f * worldSize.y + 1.f} };
	_raycaster.addBoundingBox(leftWall);
	_raycaster.addBoundingBox(rightWall);
	_raycaster.addBoundingBox(topWall);
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
