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
		void constructJumpLinks() const;
		void constructJumpLink(glm::vec2 from, glm::vec2 to) const;

		NavmeshNode* sampleNode(glm::vec2 point, float& minDistance) const;

	private:

		void runAStar(NavmeshNode* from, NavmeshNode* to, NavmeshPath& path) const;

		std::vector<Engine::BoundingBox*> _platforms;
		std::vector<std::unique_ptr<NavmeshNode>> _nodes;

		constexpr static float INF = 1000000.f;
		
		struct AStarNode
		{
			float f, g, h;
			NavmeshNode* node;
			AStarNode* parentNode;
			LinkType linkType;

			AStarNode() : f(INF), g(INF), h(INF), node(nullptr), parentNode(nullptr), linkType(WALKABLE) {}
			AStarNode(NavmeshNode* node, AStarNode* parent, LinkType type) : f(INF), g(INF), h(INF), node(node), parentNode(parent), linkType(type) {}
			explicit  AStarNode(NavmeshNode* node) : f(INF), g(INF), h(INF), node(node), parentNode(nullptr), linkType(WALKABLE) {}

			void reset()
			{
				f = g = h = INF;
				parentNode = nullptr;
				linkType = WALKABLE;
			}

			bool operator == (const AStarNode& other) const
			{
				return node == other.node;
			}
		};

		struct AStarCmp
		{
			bool operator() (const AStarNode& lhs, const AStarNode& rhs) const
			{
				return lhs.f < rhs.f;
			}

			unsigned operator() (const AStarNode& hash) const
			{
				return std::hash<NavmeshNode*>()(hash.node);
			}
		};
		
		std::unordered_map<NavmeshNode*, std::unique_ptr<AStarNode>> _graph;

	private:

		static void constructPath(NavmeshPath& path, AStarNode* endNode);
	};
}


