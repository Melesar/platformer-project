﻿#include "Enemy.h"
#include "Rendering/Sprite.h"
#include "Physics/Raycaster.h"



Platformer::Enemy::Enemy(Engine::Sprite* sprite, const Player& player, const Engine::Raycaster& raycaster) :
	_sprite(sprite),
	_player(player),
	_controller(raycaster, sprite)
{
	sprite->setLayer(Engine::BoundingBox::ENEMY);
	sprite->setSortingOrder(20);
}

void Platformer::Enemy::update(float deltaTime)
{
	_controller.move({ 0, 0 }, deltaTime);
}
