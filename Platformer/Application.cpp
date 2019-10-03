#include "Application.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"

Platformer::Application::Application()
{
	_title = "Platformer project";
	_worldHeight = 10.f;
	_isFullscreen = true;
}


void Platformer::Application::setup()
{
	Engine::Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	Engine::Sprite* playerSprite = createSprite(Engine::TEX_PLAYER, 256);
	_player = std::make_unique<Player>(playerSprite, _input, _raycaster);
	
	const glm::vec2 worldSize = { _renderer->worldWidth(), _renderer->worldHeight() };
	
	createPlatforms(worldSize);
	createWalls(worldSize);
}

void Platformer::Application::createPlatforms(const glm::vec2 worldSize)
{
	const glm::vec2 worldExtents = worldSize * 0.5f;
	createPlatform({ 0, -4.5f }, { worldSize.x, 1 });

	float platformWidth = worldSize.x * 0.333f;
	createPlatform({ -worldExtents.x + 0.5f * platformWidth, -2 }, { platformWidth, 1 });
	createPlatform({ 0, 0 }, {worldSize.x * 0.167f, 1});
	createPlatform({worldExtents.x - platformWidth * 0.5f, 1 }, { platformWidth, 1 });

	createPlatform({ -4, 2 });
	platformWidth *= 0.5f;
	createPlatform({ -worldExtents.x + platformWidth * 0.5f, 2 }, { platformWidth, 1 });
}

void Platformer::Application::createWalls(const glm::vec2 worldSize)
{
	const glm::vec2 worldExtents = worldSize * 0.5f;
	Engine::BoundingBox leftWall{ {-worldExtents.x - 1.f, -worldExtents.y}, {-worldExtents.x, worldExtents.y} };
	Engine::BoundingBox rightWall{ {worldExtents.x, -worldExtents.y}, {worldExtents.x + 1.f, worldExtents.y} };
	Engine::BoundingBox topWall{ {-worldExtents.x, worldExtents.y}, {worldExtents.x, worldExtents.y + 1.f} };
	_raycaster.addBoundingBox(leftWall);
	_raycaster.addBoundingBox(rightWall);
	_raycaster.addBoundingBox(topWall);
}

void Platformer::Application::update(float deltaTime)
{
	if (_input.keyPressed(Engine::Input::Q))
	{
		stop();
	}
	
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
