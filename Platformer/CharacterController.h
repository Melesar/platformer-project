#pragma once
#include "Core.h"
#include "Physics/Raycaster.h"


namespace Engine {
	class Sprite;
}

namespace Platformer
{
	class CharacterController
	{
	public:
		void move(glm::vec2 moveDirection, float deltaTime);
		void jump();

		bool isGrounded() const;

		CharacterController(const Engine::Raycaster& raycaster, Engine::Sprite* sprite);

	private:

		void move(glm::vec2& velocity);

		bool bottomCollisions(glm::vec2 velocity, Engine::Intersection& it) const;
		bool topCollisions(glm::vec2 velocity, Engine::Intersection& it) const;
		
	private:

		const float _moveSpeed = 3.f;
		const float _jumpHeight = 3.f;
		const float _topJumpTime = 0.5f;

		const float _skinWidth = 0.05f;

		const float _gravity = 2.f * _jumpHeight / (_topJumpTime * _topJumpTime);
		const float _jumpVelocity = _gravity * _topJumpTime;

		glm::vec2 _velocity {0};
		bool _isGrounded = false;
		
		const Engine::Raycaster& _raycaster;
		Engine::Sprite* _sprite;
	};
}


