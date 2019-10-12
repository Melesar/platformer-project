#pragma once
#include "Core.h"
#include "NavmeshNode.h"

namespace Platformer
{
	class Navmesh
	{
	public:

		bool getPath(glm::vec2 from, glm::vec2 to, NavmeshPath& path) const;
		
		void addPlatform(const Engine::BoundingBox& box);
		void build(glm::vec2 worldSize);


		void draw(const glm::mat3x3& matrix) const;

	private:

		bool isPlatform(glm::vec2 point) const;
		
		void constructNavmesh(glm::vec2 worldSize);
		void constructFallLinks();

		NavmeshNode* sampleNode(glm::vec2 point, float& minDistance) const;

	private:

		void runAStar(NavmeshNode* from, NavmeshNode* to, NavmeshPath& path) const;

		std::vector<Engine::BoundingBox*> _platforms;
		std::vector<std::unique_ptr<NavmeshNode>> _nodes;

		struct AStarNode
		{
			float f, g, h;
			NavmeshNode* node;
			AStarNode* parentNode;
			LinkType linkType;

			AStarNode(NavmeshNode* node, AStarNode* parent, LinkType type) : f(0.f), g(0.f), h(0.f), node(node), parentNode(parent), linkType(type) {}

			bool operator == (const AStarNode& other) const
			{
				return node == other.node;
			}
		};

		struct AStarCmp
		{
			bool operator() (AStarNode* lhs, AStarNode* rhs) const
			{
				return lhs->f < rhs->f;
			}
		};

	private:

		static void constructPath(NavmeshPath& path, AStarNode* endNode);
	};
}


