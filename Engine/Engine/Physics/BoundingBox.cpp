#include "Core.h"
#include "BoundingBox.h"
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

bool Engine::BoundingBox::intersects(const Ray& ray, Intersection& intersection) const
{
	glm::vec2 invDir = ray.getInvDirection();
	glm::vec2 orig = ray.getOrigin();
	
	float t0x = (min.x - orig.x) * invDir.x;
	float t0y = (min.y - orig.y) * invDir.y;
	float t1x = (max.x - orig.x) * invDir.x;
	float t1y = (max.y - orig.y) * invDir.y;

	float tmin = glm::max(glm::min(t0x, t1x), glm::min(t0y, t0y));
	float tmax = glm::min(glm::max(t0x, t1x), glm::max(t0y, t1y));

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

	intersection.distance = tmin;
	return true;
}

bool Engine::BoundingBox::operator==(const BoundingBox& other) const
{
	return min == other.min && max == other.max;
}
