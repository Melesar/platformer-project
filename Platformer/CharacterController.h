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
		void move(glm::vec2 moveDirection);
		void jump();
		void jump(glm::vec2 from, glm::vec2 to);

		bool isGrounded() const;

		void setValues(float moveSpeed, float jumpHeight, float topJumpTime);

		void update(float deltaTime);
		
		CharacterController(const Engine::Raycaster& raycaster, Engine::Sprite* sprite);

	private:

		void moveOneFrame(glm::vec2& velocity);

		bool bottomCollisions(glm::vec2 velocity, Engine::Intersection& it) const;
		bool topCollisions(glm::vec2 velocity, Engine::Intersection& it) const;
		bool rightCollisions(glm::vec2 velocity, Engine::Intersection& it) const;
		bool leftCollisions(glm::vec2 velocity, Engine::Intersection& it) const;

		void jumpDebugWindow() const;
		
	private:

		float _moveSpeed;
		float _jumpHeight;
		float _topJumpTime;
		
		const float _skinWidth = 0.05f;

		float _gravity;
		float _jumpVelocity;

		const int _verticalRays = 4;
		const int _horizontalRays = 4;
		
		const Engine::BoundingBox& _bounds;

		const float _horizontalRaySpacing = _bounds.getHeight() / _horizontalRays;
		const float _verticalRaySpacing = _bounds.getWidth() / _verticalRays;

		glm::vec2 _velocity {0};
		bool _isGrounded = false;
		
		const Engine::Raycaster& _raycaster;
		Engine::Sprite* _sprite;
	};
}


