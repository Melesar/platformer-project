#include "Player.h"

Platformer::Player::Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster) :
	_sprite(sprite),
	_input(input),
	_raycaster(raycaster),
	_controller(raycaster, sprite)
{
	_sprite->setSortingOrder(50);
	_sprite->layer = Engine::BoundingBox::PLAYER;
}


void Platformer::Player::update(float deltaTime)
{
	if (_input.isJump())
	{
		_controller.jump();
	}
	
	_controller.move(_input.getMoveDirection(), deltaTime);
}

Platformer::Player::~Player()
{
}
