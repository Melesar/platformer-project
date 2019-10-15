#pragma once
#include "Core.h"
#include "Physics/BoundingBox.h"
#include "Physics/IPhysicsBody.h"
#include <bitset>

namespace Engine
{
	class Ray;
	
	class ENGINE_API Raycaster
	{
		
	public:
		Raycaster();
		
		using LayerMask = std::bitset<BoundingBox::NUM_LAYERS>;

		void addBoundingBox(const IPhysicsBody& body);
		void addBoundingBox(const BoundingBox& box);
		void removeBoundingBox(const IPhysicsBody& body);
		
		bool raycast(const Ray& ray, Intersection& i) const;
		bool raycast(const Ray& ray, float maxDistance, Intersection& i) const;
		bool raycast(const Ray& ray, float maxDistance, BoundingBox::Layer layer, Intersection& i) const;
		bool raycast(const Ray& ray, float maxDistance, LayerMask layers, Intersection& i) const;

		static LayerMask layerToMask(BoundingBox::Layer layer);
		static LayerMask layersToMask(std::initializer_list<BoundingBox::Layer> layers);
		
	private:
		
		std::vector<std::vector<const BoundingBox*>> _boxes;
	};
}


