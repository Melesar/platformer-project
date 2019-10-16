#include "Navmesh.h"
#include "Core.h"
#include <unordered_set>

bool Platformer::Navmesh::getPath(glm::vec2 from, glm::vec2 to, NavmeshPath& path) const
{
	float d;
	NavmeshNode* targetNode = sampleNode(to, d);
	if (targetNode == nullptr)
	{
		return false;
	}

	NavmeshNode* startNode = sampleNode(from, d);
	if (startNode == nullptr ||
		!path.empty() && path.front().from == startNode && path.back().to == targetNode)
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

auto Platformer::Navmesh::constructJumpLinks() const -> void
{
	float d;
	glm::vec2 newNodePosition = { -2.f, -3.5f };
	NavmeshNode* node = sampleNode({ -2.5f, -3.5f }, d);
	node->position = newNodePosition;
	
	//Floor to platform on the left
	constructJumpLink(newNodePosition, { -3.5f, -1.f });
	//Platform on the left to center
	constructJumpLink({ -3.5f, -1.f }, { -0.5f, 1.f });

	//Center to right and back
	constructJumpLink({ 0.5f, 1.f }, { 2.5f, 3.f });
	constructJumpLink({ 2.5f, 3.f }, { 0.5f, 1.f });

	//Center to single and back
	constructJumpLink({ -0.5f, 1.f }, { -3.5f, 3.f });
	constructJumpLink({ -3.5f, 3.f }, { -0.5f, 1.f });

	//Single to upper left and back
	constructJumpLink({ -3.5f, 3.f }, { -6.5f, 3.f });
	constructJumpLink({ -6.5f, 3.f }, { -3.5f, 3.f });
}

void Platformer::Navmesh::constructJumpLink(glm::vec2 from, glm::vec2 to) const
{
	float d;
	NavmeshNode* fromNode = sampleNode(from, d);
	NavmeshNode* toNode = sampleNode(to, d);
	fromNode->transitions.insert({ JUMP, toNode });
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
	if (from == to)
	{
		return;
	}
	
	std::unordered_map<NavmeshNode*, std::unique_ptr<AStarNode>> graph;
	for (const std::unique_ptr<NavmeshNode>& node : _nodes)
	{
		graph.insert({ node.get(), std::make_unique<AStarNode>(node.get()) });
	}
	
	std::set<AStarNode, AStarCmp> openSet;
	std::unordered_set<AStarNode, AStarCmp> closedSet;

	AStarNode* start = graph.find(from)->second.get();
	start->g = 0.f;
	start->f = 0.f;
	openSet.insert(*start);
	
	while(!openSet.empty())
	{
		AStarNode q = *openSet.begin();
		AStarNode* qptr = graph.find(q.node)->second.get();
		openSet.erase(openSet.begin());

		for(auto pair : q.node->transitions)
		{
			if (pair.second == to)
			{
				std::unique_ptr<AStarNode>& goal = graph.find(pair.second)->second;
				goal->parentNode = qptr;
				goal->linkType = pair.first;
				constructPath(path, goal.get());
				return;
			}

			std::unique_ptr<AStarNode>& successor = graph.find(pair.second)->second;
			if (closedSet.find(*successor) != closedSet.end())
			{
				continue;
			}

			float g = q.g + pair.first;
			if (g < successor->g)
			{
				successor->g = g;
				successor->h = glm::distance(to->position, successor->node->position);
				successor->f = successor->g + successor->h;
				successor->parentNode = qptr;
				successor->linkType = pair.first;

				if (openSet.find(*successor) == openSet.end())
				{
					openSet.insert(*successor);
				}
			}
		}

		closedSet.insert(q);
	}
}

void Platformer::Navmesh::constructPath(NavmeshPath& path, AStarNode* endNode)
{
	AStarNode* currentNode = endNode;
	while (currentNode->parentNode != nullptr)
	{
		path.emplace_front(currentNode->parentNode->node, currentNode->node, currentNode->linkType);
		currentNode = currentNode->parentNode;
	}
}

void Platformer::Navmesh::build(glm::vec2 worldSize)
{
	constructNavmesh(worldSize);
	constructFallLinks();
	constructJumpLinks();
}

void Platformer::Navmesh::draw(const glm::mat3x3& matrix) const
{
	for (const std::unique_ptr<NavmeshNode>& node : _nodes)
	{
		glm::vec3 nodeColor{};
		switch (node->type)
		{
			case REGULAR:  nodeColor = {1, 1, 1}; break;
			case LEFT:     nodeColor = {0, 1, 0}; break;
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

		auto jumpLinksRange = node->transitions.equal_range(JUMP);
		for (auto i = jumpLinksRange.first; i != jumpLinksRange.second; ++i)
		{
			drawLine(node->position, i->second->position, matrix, { 1, 0, 1 });
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
