#pragma once
#include "Player/Player.h"
#include "../Engine/Engine/Application.h"
#include "Bullet.h"
#include "AI/Enemy.h"
#include "AI/Navigation/Navmesh.h"


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

		void createWalls(const glm::vec2 worldSize);
		void createPlatforms(const glm::vec2 worldSize);

		void spawnBullet(glm::vec2 position, glm::vec2 direction);
		void updateBullets(float deltaTime);

		void spawnEnemy(glm::vec2 position);
		
	private:

		Navmesh _navmesh;
		
		std::unique_ptr<Player> _player;
		std::vector<Bullet> _bullets;
		std::vector<std::unique_ptr<Engine::BoundingBox>> _walls;
		std::vector<std::unique_ptr<Enemy>> _enemies;
	};
}



