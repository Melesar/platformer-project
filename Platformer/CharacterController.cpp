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
			_velocity.y = 0;
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

	if (velocity.x > 0 && rightCollisions(velocity, i))
	{
		velocity.x = i.distance - _skinWidth;
	}
	else if (velocity.x < 0 && leftCollisions(velocity, i))
	{
		velocity.x = -(i.distance - _skinWidth);
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

bool Platformer::CharacterController::rightCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.max + glm::vec2(-_skinWidth, 0);
	Engine::Ray r(origin, { 1, 0 });

	const float maxDistance = glm::abs(velocity.x) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _horizontalRays + 1; ++i)
	{
		r.setOrigin(origin - glm::vec2(0, i * _horizontalRaySpacing));

		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
}

bool Platformer::CharacterController::leftCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.min + glm::vec2(_skinWidth, 0);
	Engine::Ray r(origin, { -1, 0 });

	const float maxDistance = glm::abs(velocity.x) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _horizontalRays + 1; ++i)
	{
		r.setOrigin(origin + glm::vec2(0, i * _horizontalRaySpacing));

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
		_velocity.y = _jumpVelocity;
	}
}

bool Platformer::CharacterController::isGrounded() const
{
	return _isGrounded;
}

Platformer::CharacterController::CharacterController(const Engine::Raycaster& raycaster, Engine::Sprite* sprite) :
	_bounds(sprite->getBoundingBox()),
	_raycaster(raycaster),
	_sprite(sprite)
{
}


