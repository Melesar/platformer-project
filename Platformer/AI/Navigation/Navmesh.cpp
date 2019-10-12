﻿#include "Navmesh.h"
#include "Core.h"

bool Platformer::Navmesh::getPath(glm::vec2 from, glm::vec2 to, NavmeshPath& path) const
{
	float d;
	NavmeshNode* targetNode = sampleNode(to, d);
	if (targetNode == nullptr || !path.empty() && path.back().to == targetNode)
	{
		return false;
	}

	NavmeshNode* startNode = sampleNode(from, d);
	if (startNode == nullptr)
	{
		return false;
	}

	//clear the path
	NavmeshPath().swap(path);

	runAStar(startNode, targetNode, path);

	return true;
}

void Platformer::Navmesh::addPlatform(const Engine::BoundingBox& box)
{
	_platforms.push_back(const_cast<Engine::BoundingBox*>(&box));
}

void Platformer::Navmesh::constructNavmesh(glm::vec2 worldSize)
{
	glm::vec2 worldExtents = worldSize * 0.5f;
	float borderX = worldExtents.x - (worldExtents.x - (int)worldExtents.x - 0.5f);
	for (Engine::BoundingBox* platform : _platforms)
	{
		const float xStart = glm::max(platform->min.x + 0.5f, -borderX);
		const float xEnd = glm::min(platform->max.x - 0.5f, borderX);

		for(float x = xStart; x <= xEnd; x += 1.f)
		{
			NodeType type;
			if (platform->getWidth() <= 1.f)
			{
				type = SOLO;
			}
			else if (x == xStart && xStart > -borderX)
			{
				type = LEFT;
			}
			else if (x == xEnd && xEnd < borderX)
			{
				type = RIGHT;
			}
			else
			{
				type = REGULAR;
			}

			glm::vec2 nodePosition{ x, platform->max.y + 0.5f };
			std::unique_ptr<NavmeshNode> node = std::make_unique<NavmeshNode>(nodePosition, type);
			if (type == REGULAR && x > -borderX || type == RIGHT )
			{
				_nodes[_nodes.size() - 1]->transitions.insert({ WALKABLE, node.get() });
				node->transitions.insert({ WALKABLE, _nodes[_nodes.size() - 1].get() });
			}

			_nodes.emplace_back(std::move(node));
		}
	}
}

void Platformer::Navmesh::constructFallLinks()
{
	const auto traceNodes = [&](glm::vec2 from, NavmeshNode& fallFrom)
	{
		const int maxSteps = 15;
		float distance;
		NavmeshNode* currentNode = sampleNode(from, distance);
		
		float minDistance = distance;
		NavmeshNode* closestNode = currentNode;
		
		int steps = 1;
		while(steps < maxSteps)
		{
			from += glm::vec2{ 0, -1.f };
			currentNode = sampleNode(from, distance);
			if (distance < minDistance)
			{
				minDistance = distance;
				closestNode = currentNode;
			}
			steps += 1;
		}

		if (closestNode != nullptr)
		{
			fallFrom.transitions.insert({ FALL, closestNode });
		}
	};
	
	for (std::unique_ptr<NavmeshNode>& node : _nodes)
	{
		switch (node->type)
		{
		case LEFT:
			traceNodes(node->position + glm::vec2(-1, -2), *node);
			break;
		case RIGHT:
			traceNodes(node->position + glm::vec2(+1, -2), *node);
			break;
		case SOLO:
			traceNodes(node->position + glm::vec2(-1, -2), *node);
			traceNodes(node->position + glm::vec2(+1, -2), *node);
			break;
		default: break;
		}
	}
}

Platformer::NavmeshNode* Platformer::Navmesh::sampleNode(glm::vec2 point, float& minDistance) const
{
	const float pointSampleDistance = 1.5f;

	minDistance = pointSampleDistance;
	NavmeshNode* result = nullptr;
	for (const std::unique_ptr<NavmeshNode>& node : _nodes)
	{
		float distance = glm::distance(node->position, point);
		if (distance < minDistance)
		{
			minDistance = distance;
			result = node.get();
		}
	}
	
	return result;
}

void Platformer::Navmesh::runAStar(NavmeshNode* from, NavmeshNode* to, NavmeshPath& path) const
{
	std::vector<std::unique_ptr<AStarNode>> processedNodes;
	std::set<AStarNode*, AStarCmp> openSet;
	std::set<AStarNode*, AStarCmp> closedSet;

	std::unique_ptr<AStarNode> start = std::make_unique<AStarNode>(from, nullptr, WALKABLE);
	openSet.insert(start.get());
	processedNodes.emplace_back(std::move(start));
	
	while(!openSet.empty())
	{
		AStarNode* q = *openSet.begin();
		openSet.erase(openSet.begin());

		for(auto pair : q->node->transitions)
		{
			if (pair.second == to)
			{
				std::unique_ptr<AStarNode> goal = std::make_unique<AStarNode>(pair.second, q, pair.first);
				constructPath(path, goal.get());
				return;
			}

			std::unique_ptr<AStarNode> successor = std::make_unique<AStarNode>(pair.second, q, pair.first);
			successor->g = q->g + pair.first;
			successor->h = glm::distance(to->position, successor->node->position);
			successor->f = successor->g + successor->h;
			successor->parentNode = q;
			successor->linkType = pair.first;


			auto checkSuccessor = [](const std::set<AStarNode*, AStarCmp>& set, AStarNode* successor) -> bool
			{
				auto iter = set.find(successor);
				if (iter != set.end() && (*iter)->f < successor->f)
				{
					return false;
				}

				return true;
			};

			if (checkSuccessor(openSet, successor.get()) && checkSuccessor(closedSet, successor.get()))
			{
				openSet.insert(successor.get());
			}
			
			processedNodes.emplace_back(std::move(successor));
		}

		closedSet.insert(q);
	}
}

void Platformer::Navmesh::constructPath(NavmeshPath& path, AStarNode* endNode)
{
	AStarNode* currentNode = endNode;
	while (currentNode->parentNode != nullptr)
	{
		path.emplace(currentNode->parentNode->node, currentNode->node, currentNode->linkType);
		currentNode = currentNode->parentNode;
	}
}

void Platformer::Navmesh::build(glm::vec2 worldSize)
{
	constructNavmesh(worldSize);
	constructFallLinks();
}

void Platformer::Navmesh::draw(const glm::mat3x3& matrix) const
{
	for (const std::unique_ptr<NavmeshNode>& node : _nodes)
	{
		glm::vec3 nodeColor{};
		switch (node->type)
		{
			case REGULAR:  nodeColor = {1, 1, 1}; break;
			case LEFT:     nodeColor = { 0, 1, 0 }; break;
			case RIGHT:    nodeColor = {0, 0, 1}; break;
			default: break;
		}
		drawQuad(node->position, { 0.1f, 0.1f }, matrix, nodeColor);

		auto walkableLinksRange = node->transitions.equal_range(WALKABLE);
		for (auto i = walkableLinksRange.first; i != walkableLinksRange.second; ++i)
		{
			drawLine(node->position, i->second->position, matrix, { 1, 1, 1 });
		}
		
		auto fallLinksRange = node->transitions.equal_range(FALL);
		for(auto i = fallLinksRange.first; i != fallLinksRange.second; ++i)
		{
			drawLine(node->position, i->second->position, matrix, { 0, 1, 0 });
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
