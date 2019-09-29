#include "Core.h"
#include "Ray.h"

Engine::Ray::Ray(const glm::vec2& origin, const glm::vec2& direction) :
	_origin(origin),
	_direction(glm::normalize(direction)),
	_invDirection(1.f / _direction)
{
}

glm::vec2 Engine::Ray::getOrigin() const
{
	return _origin;
}

void Engine::Ray::setOrigin(const glm::vec2& origin)
{
	_origin = origin;
}

glm::vec2 Engine::Ray::getDirection() const
{
	return _direction;
}

void Engine::Ray::setDirection(const glm::vec2& direction)
{
	_direction = glm::normalize(direction);
	_invDirection = 1.f / _direction;
}

glm::vec2 Engine::Ray::getInvDirection() const
{
	return _invDirection;
}
