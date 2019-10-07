#include "Player.h"

Platformer::Player::Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster) :
	_sprite(sprite),
	_input(input),
	_raycaster(raycaster),
	_controller(raycaster, sprite)
{
	_sprite->setPosition({ 1, -3.5 });
	_sprite->setSortingOrder(50);
	_sprite->setLayer(Engine::BoundingBox::PLAYER);
}


void Platformer::Player::update(float deltaTime)
{
	if (_input.isJump())
	{
		_controller.jump();
	}
	
	_controller.move(_input.getMoveDirection(), deltaTime);
}

glm::vec2 Platformer::Player::getPosition() const
{
	return _sprite->getPosition();
}

Platformer::Player::~Player()
{
}
