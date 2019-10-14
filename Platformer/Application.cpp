#include "Application.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

#define DRAW_NAVMESH false
#define DRAW_IMGUI_DEMO false

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

	spawnEnemy({ -6, -1 });
	
	const glm::vec2 worldSize = { _renderer->worldWidth(), _renderer->worldHeight() };
	
	createPlatforms();
	createWalls(worldSize);

	_navmesh.build(worldSize);
}

void Platformer::Application::createPlatforms()
{
	createPlatform({ 0, -4.5f }, { 100, 1 });

	createPlatform({ -3, -2 }, { 10, 1 }, {1, 0.5});
	createPlatform({ 0, 0 }, {2, 1});
	createPlatform({2, 2 }, { 10, 1 }, {0, 0.5f});

	createPlatform({ -3, 2 }, {1, 1}, {1, 0.5f});
	createPlatform({ -6, 2 }, { 10, 1 }, {1, 0.5});
}

void Platformer::Application::spawnBullet(glm::vec2 position, glm::vec2 direction)
{
	Bullet bullet;
	bullet.sprite = createSprite(Engine::TEX_BULLET, 512);
	bullet.sprite->setPosition(position);
	bullet.sprite->setSortingOrder(40);

	const float angle = -glm::degrees(atan2f(direction.y, direction.x));
	bullet.sprite->setRotation(angle);
	bullet.direction = direction;

	_bullets.push_back(bullet);
}

void Platformer::Application::updateBullets(float deltaTime)
{
	for(unsigned i = 0; i < _bullets.size();)
	{
		if (_bullets[i].sprite == nullptr)
		{
			_bullets.erase(_bullets.begin() + i);
		}
		else
		{
			++i;
		}
	}
	
	for (Bullet& bullet : _bullets)
	{
		Engine::Intersection i{};
		glm::vec2 offset = bullet.speed * deltaTime * bullet.direction;
		
		if (_raycaster.raycast(Engine::Ray(bullet.sprite->getPosition(), bullet.direction), glm::length(offset), Engine::BoundingBox::PLATFORM, i))
		{
			destroySprite(bullet.sprite);
			bullet.sprite = nullptr;
		}
		else
		{
			bullet.sprite->move(offset);
		}
	}
}

void Platformer::Application::spawnEnemy(glm::vec2 position)
{
	Engine::Sprite* enemySprite = createSprite(Engine::TEX_ENEMY, 256);
	enemySprite->setPosition(position);
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(enemySprite, *_player, _raycaster, _navmesh);
	_enemies.push_back(std::move(enemy));
}

void Platformer::Application::createWalls(const glm::vec2 worldSize)
{
	_walls.resize(3);
	
	const glm::vec2 worldExtents = worldSize * 0.5f;
	_walls[0] = std::make_unique<Engine::BoundingBox>(glm::vec2(-worldExtents.x - 1.f, -worldExtents.y), glm::vec2( -worldExtents.x, worldExtents.y ));
	_walls[1] = std::make_unique<Engine::BoundingBox>(glm::vec2(worldExtents.x, -worldExtents.y), glm::vec2(worldExtents.x + 1.f, worldExtents.y));
	_walls[2] = std::make_unique<Engine::BoundingBox>(glm::vec2(-worldExtents.x, worldExtents.y), glm::vec2(worldExtents.x, worldExtents.y + 1.f));
	
	_raycaster.addBoundingBox(*_walls[0]);
	_raycaster.addBoundingBox(*_walls[1]);
	_raycaster.addBoundingBox(*_walls[2]);
}

void Platformer::Application::update(float deltaTime)
{
	if (_input.keyPressed(Engine::Input::Q))
	{
		stop();
	}
	
	_player->update(deltaTime);

	if (_input.mouseButtonPressed(Engine::Input::LEFT))
	{
		const glm::vec2 shotPosition = _renderer->screenToWorldPos(_input.mouseCoords());
		std::cout << shotPosition << std::endl;
	}

	/*if (_input.mouseButtonPressed(Engine::Input::LEFT))
	{
		const glm::vec2 shotPosition = _renderer->screenToWorldPos(_input.mouseCoords());
		const glm::vec2 shotDirection = glm::normalize(shotPosition - _player->getPosition());
		spawnBullet(_player->getPosition(), shotDirection);
	}

	updateBullets(deltaTime);*/

	for (const auto& enemy : _enemies)
	{
		enemy->update(deltaTime);
	}

	if (DRAW_NAVMESH)
	{
		_navmesh.draw(_renderer->viewMatrix());
	}

	if (DRAW_IMGUI_DEMO)
	{
		ImGui::ShowDemoWindow();
	}
}

void Platformer::Application::onExit()
{
	Engine::Application::onExit();
}

Engine::Sprite* Platformer::Application::createPlatform(glm::vec2 position)
{
	return createPlatform(position, { 1, 1 }, { 0.5f, 0.5f });
}

Engine::Sprite* Platformer::Application::createPlatform(glm::vec2 position, glm::vec2 size)
{
	return createPlatform(position, size, { 0.5f, 0.5f });
}

Engine::Sprite* Platformer::Application::createPlatform(glm::vec2 position, glm::vec2 size, glm::vec2 pivot)
{
	Engine::Sprite* platform = createSprite(Engine::TEX_PLATFORM, 256);
	platform->setLayer(Engine::BoundingBox::PLATFORM);
	platform->setSortingOrder(10);
	platform->setSize(size);


	position += (-pivot + 0.5f) * size;
	platform->setPosition(position);

	_navmesh.addPlatform(platform->getBoundingBox());

	return platform;
}
