#include "Core.h"
#include "BoundingBox.h"
#include "Physics/Ray.h"
#include "Physics/Intersection.h"


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

float Engine::BoundingBox::getWidth() const
{
	return glm::abs(max.x - min.x);
}

float Engine::BoundingBox::getHeight() const
{
	return glm::abs(max.y - min.y);
}

bool Engine::BoundingBox::operator==(const BoundingBox& other) const
{
	return min == other.min && max == other.max;
}
