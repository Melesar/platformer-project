#include "Navmesh.h"
#include "Core.h"

void Platformer::Navmesh::addPlatform(const Engine::BoundingBox& box)
{
	_platforms.push_back(const_cast<Engine::BoundingBox*>(&box));
}

void Platformer::Navmesh::constructNavmesh(glm::vec2 worldSize)
{
	glm::vec2 worldExtents = worldSize * 0.5f;
	for (float y = worldExtents.y - 0.5f; y >= -worldExtents.y + 0.5f; y -= 1.f)
	{
		bool connectWalkable;
		for(float x = -worldExtents.x + 0.5f; x <= worldExtents.x - 0.5f; x += 1.f)
		{
			if (isPlatform({x, y}) || !isPlatform({x, y - 1}))
			{
				continue;
			}

			NodeType nodeType;
			if (!isPlatform({x - 1, y -1}) &&
				!isPlatform({x + 1, y - 1}))
			{
				connectWalkable = false;
				nodeType = SOLO;
			}
			else if (!isPlatform({x - 1, y - 1}))
			{
				connectWalkable = false;
				nodeType = LEFT;
			}
			else if (!isPlatform({x + 1, y - 1}))
			{
				connectWalkable = true;
				nodeType = RIGHT;
			}
			else
			{
				connectWalkable = true;
				nodeType = REGULAR;
			}

			_nodes.emplace_back(glm::vec2(x, y), nodeType);

			unsigned size = _nodes.size();
			if (connectWalkable && size > 1)
			{
				_nodes[size - 2].transitions.insert({ WALKABLE, &_nodes[size - 1] });
			}
		}
	}
}

void Platformer::Navmesh::constructFallLinks()
{
	const auto traceNodes = [&](glm::vec2 from, NavmeshNode& fallFrom)
	{
		const int maxSteps = 15;
		NavmeshNode* closest = sampleNode(from);
		int steps = 1;
		while(steps < maxSteps && closest == nullptr)
		{
			from += glm::vec2{ 0, -steps };
			steps += 1;
		}

		if (closest != nullptr)
		{
			fallFrom.transitions.insert({ FALL, closest });
		}
	};
	
	for (NavmeshNode& node : _nodes)
	{
		switch (node.type)
		{
		case LEFT:
			traceNodes(node.position + glm::vec2(-1, -2), node);
			break;
		case RIGHT:
			traceNodes(node.position + glm::vec2(+1, -2), node);
			break;
		case SOLO:
			traceNodes(node.position + glm::vec2(-1, -2), node);
			traceNodes(node.position + glm::vec2(+1, -2), node);
			break;
		default: break;
		}
	}
}

Platformer::NavmeshNode* Platformer::Navmesh::sampleNode(glm::vec2 point) const
{
	const float pointSampleDistance = 1.5f;

	float minDistance = pointSampleDistance;
	NavmeshNode* result = nullptr;
	for (const NavmeshNode& node : _nodes)
	{
		float distance = glm::distance(node.position, point);
		if (distance < minDistance)
		{
			minDistance = distance;
			result = const_cast<NavmeshNode*>(&node);
		}
	}

	return result;
}

void Platformer::Navmesh::build(glm::vec2 worldSize)
{
	constructNavmesh(worldSize);
	constructFallLinks();
}

void Platformer::Navmesh::draw(const glm::mat3x3& matrix) const
{
	for (const NavmeshNode& node : _nodes)
	{
		drawQuad(node.position, { 0.1f, 0.1f }, matrix, { 1, 0, 0 });

		auto walkableLinksRange = node.transitions.equal_range(WALKABLE);
		for (auto i = walkableLinksRange.first; i != walkableLinksRange.second; ++i)
		{
			drawLine(node.position, i->second->position, matrix, { 1, 1, 1 });
		}
		
		auto fallLinksRange = node.transitions.equal_range(FALL);
		for(auto i = fallLinksRange.first; i != fallLinksRange.second; ++i)
		{
			drawLine(node.position, i->second->position, matrix, { 0, 1, 0 });
		}
	}
}

bool Platformer::Navmesh::isPlatform(glm::vec2 point) const
{
	for (Engine::BoundingBox* const platform : _platforms)
	{
		if (platform->isPointInside(point))
		{
			return true;
		}
	}

	return false;
}
