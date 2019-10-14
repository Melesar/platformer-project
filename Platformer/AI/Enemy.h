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

		Enemy(Engine::Sprite* sprite, const Player& player, const Engine::Raycaster& raycaster, const Navmesh& navmesh);
		virtual ~Enemy();
		
		void update(float deltaTime);

		float getHealth() const;
		float getMaxHealth() const;

		void takeDamage(float damage);
		
	private:

		void moveToPoint(glm::vec2 destination);
		void moveInDirection(short direction);
		
	private:

		const float MAX_HEALTH = 30.f;
		
		float _health {MAX_HEALTH};
		
		
		const Player& _player;
		CharacterController _controller;
		
		const Navmesh& _navmesh;
		NavmeshPath _currentPath;
		NavmeshLink* _currentTarget;
	};
}


