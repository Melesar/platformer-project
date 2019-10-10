#include "Core.h"
#include "Enemy.h"
#include "Rendering/Sprite.h"
#include "Physics/Raycaster.h"
#include "Player/Player.h"
#include "AI/AIMovementConstants.h"


Platformer::Enemy::Enemy(Engine::Sprite* sprite, const Player& player, const Engine::Raycaster& raycaster) :
	_sprite(sprite),
	_player(player),
	_controller(raycaster, sprite)
{
	sprite->setLayer(Engine::BoundingBox::ENEMY);
	sprite->setSortingOrder(20);

	_controller.setValues(MOVEMENT_SPEED, JUMP_HEIGHT, JUMP_TIME);
}

void Platformer::Enemy::update(float deltaTime)
{

}

void Platformer::Enemy::moveToPoint(glm::vec2 destination, float deltaTime)
{
	_controller.move({ glm::sign(destination.x - _sprite->getPosition().x), 0 }, deltaTime);
}

void Platformer::Enemy::moveInDirection(short direction, float deltaTime)
{
	_controller.move({ direction, 0.f }, deltaTime);
}
