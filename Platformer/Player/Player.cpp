#include "Player.h"
#include "Player/PlayerMovementConstants.h"

Platformer::Player::Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster) :
	VisibleEntity(sprite),
	_input(input),
	_raycaster(raycaster),
	_controller(raycaster, sprite)
{
	_sprite->setPosition({ 0.5, -3.5 });
	_sprite->setSortingOrder(50);
	_sprite->setLayer(Engine::BoundingBox::PLAYER);
	_sprite->setOwner(this);

	_controller.setValues(MOVEMENT_SPEED, JUMP_HEIGHT, JUMP_TIME);
}


void Platformer::Player::update(float deltaTime)
{
	if (_input.isJump())
	{
		_controller.jump();
	}
	
	_controller.move(_input.getMoveDirection());
	_controller.update(deltaTime);

	if (_isInvincible)
	{
		_invincibilityTimer += deltaTime;
		_currentAlpha = 0.5f * (glm::cos(_invincibilityTimer * 25.f) + 1);
		_sprite->setColor(Engine::Color{ 1, 1, 1, _currentAlpha });
		if (_invincibilityTimer >= _invincibilityTime)
		{
			_isInvincible = false;
			_sprite->setColor(Engine::Color{ 1, 1, 1, 1 });
		}
	}
}

void Platformer::Player::damage()
{
	if (!_isInvincible)
	{
		_lives = glm::max(0, _lives - 1);
		_isDamaged = true;
		_isInvincible = true;
		_invincibilityTimer = 0.f;
	}
}

bool Platformer::Player::isDamaged()
{
	if (_isDamaged)
	{
		_isDamaged = false;
		return true;
	}

	return false;
}

int Platformer::Player::getMaxLives() const
{
	return _maxLives;
}

int Platformer::Player::getLives() const
{
	return _lives;
}

glm::vec2 Platformer::Player::getPosition() const
{
	return _sprite->getPosition();
}

Platformer::Player::~Player()
{
}
