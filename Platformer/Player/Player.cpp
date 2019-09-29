#include "Player.h"

Platformer::Player::Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster) :
	_sprite(sprite),
	_input(input),
	_raycaster(raycaster)
{
	_sprite->setSortingOrder(50);
	_sprite->layer = Engine::BoundingBox::PLAYER;
}



void Platformer::Player::update(float deltaTime)
{
	const float moveSpeed = 3.f;
	glm::vec2 offset = moveSpeed * deltaTime * _input.getMoveDirection();

	_sprite->move(offset);
	if (offset.x != 0)
	{
		_sprite->flipX(offset.x < 0);
	}
}

Platformer::Player::~Player()
{
}
