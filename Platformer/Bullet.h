#pragma once
#include "Rendering/Sprite.h"

namespace Platformer
{
	struct Bullet
	{
		Engine::Sprite* sprite;
		glm::vec2 direction;

		const float speed = 5.f;

		Bullet& operator = (const Bullet& other)
		{
			sprite = other.sprite;
			direction = other.direction;
			return *this;
		}
	};
}
