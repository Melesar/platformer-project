#pragma once
#include "CharacterController.h"
#include "AI/Navigation/Navmesh.h"
#include "VisibleEntity.h"

namespace Engine
{
	class Sprite;
	class Raycaster;
}

namespace Platformer
{
	class Player;

	class Enemy : public Engine::VisibleEntity
	
	{
	public:

		Enemy(glm::vec2 position, Player& player, const Engine::Raycaster& raycaster, const Navmesh& navmesh);
		virtual ~Enemy();

		void attackPlayer() const;
		void update(float deltaTime);

		float getHealth() const;
		float getMaxHealth() const;

		void takeDamage(float damage);
		
	private:

		void moveToPoint(glm::vec2 destination);
		void moveInDirection(short direction);
		
	private:

		const float _maxHealth = 30.f;
		const float _attackRange = 0.2f;
		
		float _health {_maxHealth};

		Player& _player;
		const Engine::Raycaster& _raycaster;
		const Engine::BoundingBox& _boundingBox;
		CharacterController _controller;

		const Navmesh& _navmesh;
		NavmeshPath _currentPath;
		NavmeshLink* _currentTarget;
	};
}


