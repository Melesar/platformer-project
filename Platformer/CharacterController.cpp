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
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.min + glm::vec2(0, _skinWidth);
	Engine::Ray r(origin, { 0, -1 });

	const float maxDistance = glm::abs(velocity.y) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _verticalRays + 1; ++i)
	{
		r.setOrigin(origin + glm::vec2(i * _verticalRaySpacing, 0));
		
		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
}

bool Platformer::CharacterController::topCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.max + glm::vec2(0, -_skinWidth);
	Engine::Ray r(origin, { 0, 1 });

	const float maxDistance = glm::abs(velocity.y) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _verticalRays + 1; ++i)
	{
		r.setOrigin(origin - glm::vec2(i * _verticalRaySpacing, 0));
		
		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
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
	_bounds(sprite->getBoundingBox()),
	_horizontalRaySpacing(_bounds.getHeight() / _horizontalRays),
	_verticalRaySpacing(_bounds.getWidth() / _verticalRays),
	_raycaster(raycaster),
	_sprite(sprite)
{
}


