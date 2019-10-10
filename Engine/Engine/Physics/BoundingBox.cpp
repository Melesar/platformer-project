#include "Core.h"
#include "BoundingBox.h"
#include "Physics/Ray.h"
#include "Physics/Intersection.h"


glm::vec2 Engine::BoundingBox::getCenter() const
{
	return (min + max) * 0.5f;
}

bool Engine::BoundingBox::intersects(const Ray& ray, Intersection& intersection) const
{
	const glm::vec2 invDir = ray.getInvDirection();
	const glm::vec2 orig = ray.getOrigin();

	const float t0x = (min.x - orig.x) * invDir.x;
	const float t0y = (min.y - orig.y) * invDir.y;
	const float t1x = (max.x - orig.x) * invDir.x;
	const float t1y = (max.y - orig.y) * invDir.y;

	const float tmin = glm::max(glm::min(t0x, t1x), glm::min(t0y, t1y));
	const float tmax = glm::min(glm::max(t0x, t1x), glm::max(t0y, t1y));

	if (tmax < 0)
	{
		intersection.distance = tmax;
		return false;
	}

	if (tmin > tmax)
	{
		intersection.distance = tmin;
		return false;
	}

	intersection.bb = *this;
	intersection.distance = tmin;
	return intersection.distance > 0;
}

bool Engine::BoundingBox::intersects(const BoundingBox& other) const
{
	return other.max.x >= min.x && other.max.y >= min.y &&
		other.min.x <= max.x && other.min.y <= max.y;
}

bool Engine::BoundingBox::isPointInside(glm::vec2 p) const
{
	return p.x >= min.x && p.x <= max.x &&
		   p.y >= min.y && p.y <= max.y;
}

float Engine::BoundingBox::getWidth() const
{
	return glm::abs(max.x - min.x);
}

float Engine::BoundingBox::getHeight() const
{
	return glm::abs(max.y - min.y);
}

Engine::BoundingBox& Engine::BoundingBox::operator=(const BoundingBox& other)
{
	min = other.min;
	max = other.max;
	layer = other.layer;

	return *this;
}

bool Engine::BoundingBox::operator==(const BoundingBox& other) const
{
	return min == other.min && max == other.max;
}

bool Engine::BoundingBox::operator!=(const BoundingBox& other) const
{
	return !(*this == other);
}
