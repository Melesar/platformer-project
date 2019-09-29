#pragma once
#include "Core.h"


namespace Engine
{
	class ENGINE_API Ray
	{
	public:
		Ray(const glm::vec2& origin, const glm::vec2& direction);
		
		virtual ~Ray() = default;

		glm::vec2 getOrigin() const;
		glm::vec2 getDirection() const;
		glm::vec2 getInvDirection() const;
		
		void setDirection(const glm::vec2& direction);
		void setOrigin(const glm::vec2& origin);
		
	private:
		glm::vec2 _origin;
		glm::vec2 _direction;
		glm::vec2 _invDirection;
	};
}


