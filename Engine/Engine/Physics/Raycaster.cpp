#include "Core.h"
#include "Raycaster.h"
#include "Physics/Intersection.h"

Engine::Raycaster::Raycaster()
{
	_boxes.resize(BoundingBox::NUM_LAYERS);
}

void Engine::Raycaster::addBoundingBox(const IPhysicsBody& body)
{
	const BoundingBox& box = body.getBoundingBox();
	addBoundingBox(box);
}

void Engine::Raycaster::addBoundingBox(const BoundingBox& box)
{
	_boxes[box.layer].push_back(&box);
}

void Engine::Raycaster::removeBoundingBox(const IPhysicsBody& body)
{
	const BoundingBox box = body.getBoundingBox();
		std::vector<const BoundingBox*> layerBoxes = _boxes[box.layer];
	for (size_t i = 0; i < layerBoxes.size(); ++i)
	{
		if (*layerBoxes[i] == box)
		{
			layerBoxes.erase(layerBoxes.begin() + i);
			return;
		}
	}
}

bool Engine::Raycaster::raycast(const Ray& ray, Intersection& i) const
{
	return raycast(ray, 1000.f, i);
}

bool Engine::Raycaster::raycast(const Ray& ray, float maxDistance, Intersection& i) const
{
	return raycast(ray, maxDistance, BoundingBox::ALL, i);
}

bool Engine::Raycaster::raycast(const Ray& ray, float maxDistance, BoundingBox::Layer layer, Intersection& i) const
{
	if (layer != BoundingBox::ALL)
	{
		return raycast(ray, maxDistance, i, _boxes[layer]);
	}
	
	Intersection intI {};
	float minDistance = maxDistance;
	for (int index = 0; index < BoundingBox::NUM_LAYERS; ++index)
	{
		bool intersects = raycast(ray, maxDistance, intI, _boxes[index]);
		if (intersects && intI.distance < minDistance)
		{
			minDistance = intI.distance;
		}
	}

	i.bb = intI.bb;
	i.distance = minDistance;
	return minDistance < maxDistance;
}

bool Engine::Raycaster::raycast(const Ray& ray, float maxDistance, Intersection& i,
	const std::vector<const BoundingBox*>& boxes)
{
	float minDistance = maxDistance;
	Intersection intI{};
	for (const BoundingBox* box : boxes)
	{
		bool intersects = box->intersects(ray, intI);
		if (intersects && intI.distance < minDistance)
		{
			minDistance = intI.distance;
		}
	}

	i.bb = intI.bb;
	i.distance = minDistance;
	return minDistance < maxDistance;
}
