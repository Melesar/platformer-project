#pragma once
#include "Rendering/Sprite.h"
#include "Input/Input.h"
#include "Physics/Raycaster.h"
#include "CharacterController.h"
#include "VisibleEntity.h"

namespace Platformer
{
	class Player : public Engine::VisibleEntity
	{
	public:
		Player(glm::vec2 position, const Engine::Input& input, const Engine::Raycaster& raycaster);
		virtual ~Player();

		void update(float deltaTime);

		void damage();
		bool isDamaged();
		
		int getMaxLives() const;
		int getLives() const;
		glm::vec2 getPosition() const;

	private:

		const int _maxLives = 3;
		const float _invincibilityTime = 2.5f;
		
		int _lives {_maxLives};
		bool _isDamaged {false};
		bool _isInvincible{ false };
		float _invincibilityTimer = 0.f;

		float _currentAlpha = 1.f;
		
		const Engine::Input& _input;
		const Engine::Raycaster& _raycaster;

		CharacterController _controller;
	};
}


