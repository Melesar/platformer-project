#include "CharacterController.h"
#include "Rendering/Sprite.h"
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

void Platformer::CharacterController::move(glm::vec2 moveDirection, float deltaTime)
{
	const float offsetX = _moveSpeed * moveDirection.x;
	_velocity.x = offsetX;
	if (offsetX != 0)
	{
		_sprite->flipX(offsetX < 0);
	}

	_velocity.y -= _gravity * deltaTime;

	glm::vec2 frameVelocity = _velocity * deltaTime;
	move(frameVelocity);

	_sprite->move(frameVelocity);
}

void Platformer::CharacterController::move(glm::vec2& velocity)
{
	Engine::Intersection i {};
	if (velocity.y < 0)
	{
		if (bottomCollisions(velocity, i))
		{
			velocity.y = -(i.distance - _skinWidth);
			_isGrounded = true;
		}
		else
		{
			_isGrounded = false;
		}
	}
	else if (velocity.y > 0)
	{
		if (topCollisions(velocity, i))
		{
			velocity.y = i.distance - _skinWidth;
		}

		_isGrounded = false;
	}
}

bool Platformer::CharacterController::bottomCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Ray r(_sprite->min + glm::vec2(0, _skinWidth), { 0, -1 });
	return _raycaster.raycast(r, glm::abs(velocity.y) + _skinWidth, Engine::BoundingBox::PLATFORM, it);
}

bool Platformer::CharacterController::topCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Ray r(_sprite->max + glm::vec2(0, -_skinWidth), { 0, 1 });
	return _raycaster.raycast(r, glm::abs(velocity.y) + _skinWidth, Engine::BoundingBox::PLATFORM, it);
}

void Platformer::CharacterController::jump()
{
	if (isGrounded())
	{
		std::cout << "Jump" << std::endl;
		_velocity.y = _jumpVelocity;
	}
	else
	{
		std::cout << "Not grounded" << std::endl;
	}
}

bool Platformer::CharacterController::isGrounded() const
{
	return _isGrounded;
}

Platformer::CharacterController::CharacterController(const Engine::Raycaster& raycaster, Engine::Sprite* sprite) :
	_raycaster(raycaster),
	_sprite(sprite)
{
}


