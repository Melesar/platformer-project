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
	const BoundingBox& box = body.getBoundingBox();
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
	return raycast(ray, maxDistance, layerToMask(layer), i);
}

bool Engine::Raycaster::raycast(const Ray& ray, float maxDistance, LayerMask layers, Intersection& i) const
{
	float minDistance = maxDistance;
	BoundingBox closestBox;
	Intersection intI{};
	for (int layer = 0; layer < BoundingBox::NUM_LAYERS; ++layer)
	{
		if (layers.test(layer))
		{
			const std::vector<const BoundingBox*>& boxes = _boxes[layer];
			for (const BoundingBox* box : boxes)
			{
				bool intersects = box->intersects(ray, intI);
				if (intersects && intI.distance < minDistance)
				{
					minDistance = intI.distance;
					closestBox = intI.bb;
				}
			}
		}
	}

	i.bb = closestBox;
	i.distance = minDistance;
	return minDistance < maxDistance;
}

Engine::Raycaster::LayerMask Engine::Raycaster::layerToMask(BoundingBox::Layer layer)
{
	LayerMask mask;
	if (layer != BoundingBox::ALL)
	{
		mask.set(layer);
	}
	else
	{
		mask.set();
	}
	return mask;
}

Engine::Raycaster::LayerMask Engine::Raycaster::layersToMask(std::initializer_list<BoundingBox::Layer> layers)
{
	LayerMask mask;
	for (BoundingBox::Layer layer : layers)
	{
		mask.set(layer);
	}

	return mask;
}
