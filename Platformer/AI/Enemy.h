#pragma once
#include "CharacterController.h"
#include "AI/Navigation/Navmesh.h"
#include <glm/common.hpp>

namespace Engine
{
	class Sprite;
	class Raycaster;
}

namespace Platformer
{
	class Player;

	class Enemy
	{
	public:

		Enemy(Engine::Sprite* sprite, const Player& player, const Engine::Raycaster& raycaster, const Navmesh& navmesh);

		void update(float deltaTime);

	private:

		void moveToPoint(glm::vec2 destination);
		void moveInDirection(short direction);
		
	private:

		Engine::Sprite* _sprite;
		const Player& _player;
		CharacterController _controller;
		
		const Navmesh& _navmesh;
		NavmeshPath _currentPath;
		NavmeshLink* _currentTarget;
	};
}


