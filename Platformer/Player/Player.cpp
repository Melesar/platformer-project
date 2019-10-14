#include "Player.h"
#include "Player/PlayerMovementConstants.h"

Platformer::Player::Player(Engine::Sprite* sprite, const Engine::Input& input, const Engine::Raycaster& raycaster) :
	_sprite(sprite),
	_input(input),
	_raycaster(raycaster),
	_controller(raycaster, sprite)
{
	_sprite->setPosition({ 0.5, -3.5 });
	_sprite->setSortingOrder(50);
	_sprite->setLayer(Engine::BoundingBox::PLAYER);

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
}

glm::vec2 Platformer::Player::getPosition() const
{
	return _sprite->getPosition();
}

Platformer::Player::~Player()
{
}
