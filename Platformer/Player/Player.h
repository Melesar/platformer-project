#pragma once
#include "Rendering/Sprite.h"
#include "Input/Input.h"
#include "Physics/Raycaster.h"

namespace Platformer
{
	class Player
	{
	public:
		Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster);
		virtual ~Player();

		void update(float deltaTime);

	private:
		Engine::Sprite* _sprite;

		const Engine::Input& _input;
		const Engine::Raycaster& _raycaster;
	};
}


