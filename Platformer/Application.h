#pragma once
#include "Player/Player.h"
#include "../Engine/Engine/Application.h"
#include "Bullet.h"
#include "AI/Enemy.h"
#include "AI/Navigation/Navmesh.h"
#include "UI/PlayerHealthWidget.h"


namespace Platformer
{
	class Application final : public Engine::Application
	{
	public:

		Application();
		~Application() = default;

	protected:

		void setup() override;
		void update(float deltaTime) override;
		void onExit() override;

	private:

		Engine::Sprite* createPlatform(glm::vec2 position);
		Engine::Sprite* createPlatform(glm::vec2 position, glm::vec2 size);
		Engine::Sprite* createPlatform(glm::vec2 position, glm::vec2 size, glm::vec2 pivot);

		void createWalls(const glm::vec2 worldSize);
		void createPlatforms();
		void createSpawnPoints();
		
		void updateBullets(float deltaTime);
		void updateEnemies(float deltaTime);

		void spawnBullet(glm::vec2 position, glm::vec2 direction);
		void spawnEnemy(glm::vec2 position);
		
	private:

		const float _enemySpawnTime = 2.f;
		float _lastEnemySpawnTime;

		std::unique_ptr<Player> _player;
		std::unique_ptr<PlayerHealthWidget> _playerHealthWidget;
		
		std::vector<glm::vec2> _spawnPoints;
		std::vector<Bullet> _bullets;
		std::vector<std::unique_ptr<Engine::BoundingBox>> _walls;
		std::vector<std::unique_ptr<Enemy>> _enemies;

		Navmesh _navmesh;
	};
}



