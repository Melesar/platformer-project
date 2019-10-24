#pragma once
#include <Core.h>
#include "EngineEntity.h"

namespace Engine
{
	struct Intersection;
	class Ray;

	struct ENGINE_API BoundingBox
	{
		glm::vec2 min {0};
		glm::vec2 max {0};

		enum Layer
		{
			PLAYER,
			PLATFORM,
			ENEMY,
			BULLET,

			NUM_LAYERS,
			ALL
		} layer = PLATFORM;

		Entity* owner {nullptr};

		BoundingBox() = default;
		BoundingBox(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}
		BoundingBox(const BoundingBox& other) = default;

		glm::vec2 getCenter() const;
		
		bool intersects(const Ray& ray, Intersection& intersection) const;
		bool intersects(const BoundingBox& other) const;

		bool isPointInside(glm::vec2 p) const;
		
		float getWidth() const;
		float getHeight() const;

		BoundingBox& operator = (const BoundingBox& other);
		bool operator == (const BoundingBox& other) const;
		bool operator != (const BoundingBox& other) const;
	};
}

inline std::ostream& operator << (std::ostream& stream, const Engine::BoundingBox& box)
{
    stream << "Min: " << box.min << ", max: " << box.max;
    return stream;
}

