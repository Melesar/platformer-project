#include "CharacterController.h"
#include "Rendering/Sprite.h"
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

void Platformer::CharacterController::move(glm::vec2 moveDirection, float deltaTime)
{
	const float offsetX = _moveSpeed * deltaTime * moveDirection.x;
	_velocity.x = offsetX;
	if (offsetX != 0)
	{
		_sprite->flipX(offsetX < 0);
	}

	if (!isGrounded())
	{
		_velocity.y -= _gravity * deltaTime;
	}
	else
	{
		_velocity.y = 0;
	}

	move(_velocity);

	_sprite->move(_velocity);
}

void Platformer::CharacterController::move(glm::vec2& velocity)
{
	Engine::Intersection i;
	if (bottomCollisions(i))
	{
		velocity.y = -(i.distance - _skinWidth);
		_isGrounded = true;
	}
}

bool Platformer::CharacterController::bottomCollisions(Engine::Intersection& it) const
{
	Engine::Ray r(_sprite->min + glm::vec2(_skinWidth, 0.), { 0, -1 });
	return _raycaster.raycast(r, glm::abs(_velocity.y) + _skinWidth, Engine::BoundingBox::PLATFORM, it);
}

void Platformer::CharacterController::jump()
{
	if (isGrounded())
	{
		_velocity.y += _jumpVelocity;
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


