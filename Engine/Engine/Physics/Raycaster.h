#pragma once
#include "Core.h"
#include "Physics/BoundingBox.h"
#include "Physics/IPhysicsBody.h"

namespace Engine
{
	class Ray;
	
	class ENGINE_API Raycaster
	{
	public:
		Raycaster();
		
		void addBoundingBox(const IPhysicsBody& body);
		void addBoundingBox(const BoundingBox& box);
		void removeBoundingBox(const IPhysicsBody& body);
		
		bool raycast(const Ray& ray, Intersection& i) const;
		bool raycast(const Ray& ray, float maxDistance, Intersection& i) const;
		bool raycast(const Ray& ray, float maxDistance, BoundingBox::Layer layer, Intersection& i) const;

	private:

		static bool raycast(const Ray& ray, float maxDistance, Intersection& i, const std::vector<const BoundingBox*>& boxes);
		
	private:
		
		std::vector<std::vector<const BoundingBox*>> _boxes;
	};
}


