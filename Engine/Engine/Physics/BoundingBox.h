#pragma once
#include <Core.h>

namespace Engine
{
	struct Intersection;
	class Ray;

	struct ENGINE_API BoundingBox
	{
		glm::vec2 min;
		glm::vec2 max;

		enum Layer
		{
			PLAYER,
			PLATFORM,
			ENEMY,

			NUM_LAYERS,
			ALL
		} layer;

		bool intersects(const Ray& ray, Intersection& intersection) const;
		bool intersects(const BoundingBox& other) const;

		bool operator == (const BoundingBox& other) const;
	};
}

