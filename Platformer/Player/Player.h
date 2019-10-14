#pragma once
#include "Rendering/Sprite.h"
#include "Input/Input.h"
#include "Physics/Raycaster.h"
#include "CharacterController.h"
#include "PlayerShooting.h"
#include "VisibleEntity.h"

namespace Platformer
{
	class Player : public Engine::VisibleEntity
	{
	public:
		Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster);
		virtual ~Player();

		void update(float deltaTime);

		glm::vec2 getPosition() const;

	private:
		
		const Engine::Input& _input;
		const Engine::Raycaster& _raycaster;

		CharacterController _controller;
	};
}


