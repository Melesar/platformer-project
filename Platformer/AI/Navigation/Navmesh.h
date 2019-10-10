#pragma once
#include "Core.h"
#include "NavmeshNode.h"

namespace Platformer
{
	class Navmesh
	{
	public:

		void addPlatform(const Engine::BoundingBox& box);
		void build(glm::vec2 worldSize);


		void draw(const glm::mat3x3& matrix) const;

	private:

		bool isPlatform(glm::vec2 point) const;
		
		void constructNavmesh(glm::vec2 worldSize);
		void constructFallLinks();

		NavmeshNode* sampleNode(glm::vec2 point) const;

	private:

		std::vector<Engine::BoundingBox*> _platforms;
		std::vector<NavmeshNode> _nodes;
	};
}


