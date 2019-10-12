#pragma once
#include "Core.h"
#include <unordered_map>

namespace Platformer
{
	enum NodeType
	{
		NONE, REGULAR, LEFT, RIGHT, SOLO
	};

	enum LinkType
	{
		WALKABLE = 1, FALL = 2, JUMP = 5
	};

	struct NavmeshNode
	{
		glm::vec2 position;
		NodeType type;

		std::unordered_multimap<LinkType, NavmeshNode*> transitions;

		NavmeshNode() = default;
		NavmeshNode(glm::vec2 position, NodeType type) : position(position), type(type) {}
	};

	struct NavmeshLink
	{
		NavmeshNode* from;
		NavmeshNode* to;
		LinkType type;

		NavmeshLink() = default;
		NavmeshLink(NavmeshNode* from, NavmeshNode* to, LinkType type) : from(from), to(to), type(type) {}
	};

	using NavmeshPath = std::queue<NavmeshLink>;
}
