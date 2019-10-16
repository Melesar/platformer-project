#include "Application.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

#define DRAW_NAVMESH false
#define DRAW_IMGUI_DEMO false
#define IS_FULLSCREEN true
#define SHOW_STATS false


Platformer::Application::Application()
{
	_title = "Platformer project";
	_worldHeight = 10.f;
	_isFullscreen = IS_FULLSCREEN;
}

void Platformer::Application::setup()
{
	Engine::Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	const glm::vec2 worldSize = { _renderer->worldWidth(), _renderer->worldHeight() };
	
	createPlatforms();
	createWalls(worldSize);
	createSpawnPoints();

	_navmesh.build(worldSize);

	createPlayer();
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

void Platformer::Application::createSpawnPoints()
{
	float halfWidth = _renderer->worldWidth() * 0.5f;
	_spawnPoints.emplace_back(-halfWidth + 1.f, -3.5f);
	_spawnPoints.emplace_back(halfWidth - 1.f, -3.5f);
	_spawnPoints.emplace_back(halfWidth - 1.f, 3.5f);
	_spawnPoints.emplace_back(halfWidth - 1.f, 3.5f);
}

void Platformer::Application::createPlayer()
{
	_player = std::make_unique<Player>(glm::vec2{ 0.5, -3.5 }, _input, _raycaster);
	glm::vec2 widgetPosition = { 0.f, _renderer->worldHeight() * 0.5f - 0.5f };
	_playerHealthWidget = std::make_unique<PlayerHealthWidget>(widgetPosition, _player.get());
}

void Platformer::Application::spawnBullet(glm::vec2 position, glm::vec2 direction)
{
	Bullet bullet;
	bullet.sprite = createSprite(Engine::TEX_BULLET, 512);
	bullet.sprite->setPosition(position);
	bullet.sprite->setSortingOrder(40);
	bullet.sprite->setLayer(Engine::BoundingBox::BULLET);
	
	const float angle = -glm::degrees(atan2f(direction.y, direction.x));
	bullet.sprite->setRotation(angle);
	bullet.direction = direction;

	_bullets.push_back(bullet);
}

void Platformer::Application::updateBullets(float deltaTime)
{
	std::vector<unsigned> destroyedBullets;
	
	for (unsigned index = 0; index < _bullets.size(); ++index)
	{
		Bullet& bullet = _bullets[index];
		Engine::Intersection i{};
		glm::vec2 offset = bullet.speed * deltaTime * bullet.direction;

		Engine::Raycaster::LayerMask hitMask = Engine::Raycaster::layersToMask({ Engine::BoundingBox::ENEMY, Engine::BoundingBox::PLATFORM });
		if (_raycaster.raycast(Engine::Ray(bullet.sprite->getPosition(), bullet.direction), glm::length(offset), hitMask, i))
		{
			destroyedBullets.push_back(index);
			
			Enemy* hitEnemy = dynamic_cast<Enemy*>(i.bb.owner);
			if (hitEnemy != nullptr)
			{
				hitEnemy->takeDamage(16.f);
			}
		}
		else
		{
			bullet.sprite->move(offset);
		}
	}

	for (unsigned i : destroyedBullets)
	{
		destroySprite(_bullets[i].sprite);
		_bullets.erase(_bullets.begin() + i);
	}
}

void Platformer::Application::spawnEnemy(glm::vec2 position)
{
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(position, *_player, _raycaster, _navmesh);
	_enemies.push_back(std::move(enemy));
}

void Platformer::Application::restartGame()
{
	_gameOverWidget.reset();
	_isGameOver = false;
	
	_playerHealthWidget.reset();
	
	for (std::unique_ptr<Enemy>& enemy : _enemies)
	{
		enemy.reset();
	}
	_enemies.clear();
	
	_player.reset();

	_lastEnemySpawnTime = 0.f;
	
	createPlayer();
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

void Platformer::Application::updateEnemies(float deltaTime)
{
	if (_lastEnemySpawnTime < _enemySpawnTime)
	{
		_lastEnemySpawnTime += deltaTime;
	}
	else
	{
		_lastEnemySpawnTime = 0.f;
		glm::vec2 spawnPoint = *selectRandomly(_spawnPoints.begin(), _spawnPoints.end());
		spawnEnemy(spawnPoint);
	}
	
	std::vector<int> enemiesDead;
	
	for (unsigned i = 0; i < _enemies.size(); ++i)
	{
		std::unique_ptr<Enemy>& enemy = _enemies[i];
		if (enemy->getHealth() > 0.f)
		{
			enemy->update(deltaTime);
		}
		else 
		{
			enemiesDead.push_back(i);
		}
	}

	for (int i : enemiesDead)
	{
		std::unique_ptr<Enemy>& deadEnemy = _enemies[i];
		deadEnemy.reset();

		_enemies.erase(_enemies.begin() + i);
	}
}

void Platformer::Application::update(float deltaTime)
{
	if (_input.keyPressed(Engine::Input::Q))
	{
		stop();
	}

	if (_isGameOver)
	{
		if (_gameOverWidget->isButtonPressed())
		{
			restartGame();
		}
	}
	else
	{
		_player->update(deltaTime);

		if (_input.mouseButtonPressed(Engine::Input::LEFT))
		{
			const glm::vec2 shotPosition = _renderer->screenToWorldPos(_input.mouseCoords());
			const glm::vec2 shotDirection = glm::normalize(shotPosition - _player->getPosition());
			spawnBullet(_player->getPosition(), shotDirection);
		}

		updateBullets(deltaTime);
		updateEnemies(deltaTime);

		_playerHealthWidget->update();

		if (_player->getLives() == 0)
		{
			_isGameOver = true;
			_gameOverWidget = std::make_unique<GameOverWidget>(*_renderer, _input);
		}
	}
	
	if (DRAW_NAVMESH)
	{
		_navmesh.draw(_renderer->viewMatrix());
	}

	if (DRAW_IMGUI_DEMO)
	{
		ImGui::ShowDemoWindow();
	}

	if (SHOW_STATS)
	{
		showStats();
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
