#include "Platformer.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

Platformer::Platformer()
{
	_title = "Platformer project";
}

void Platformer::setup()
{
	Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	_player = createSprite(Engine::TEX_ELLIOT, 1000, 1000);
	_player->setSortingOrder(50);
	_player->layer = Engine::BoundingBox::PLAYER;

	Engine::Sprite* enemy1 = createSprite(Engine::TEX_ELLIOT, 600, 600);
	enemy1->setPosition({ -3, 2 });
	enemy1->setSortingOrder(10);
	enemy1->layer = Engine::BoundingBox::ENEMY;

	Engine::Sprite* platform = createSprite(Engine::TEX_PLATFORM, 100, 100);
	platform->setPosition({ 3, 2 });
	platform->setSize({ 2, 1 });

	_enemy = createSprite(Engine::TEX_ELLIOT, 200, 200);
	_enemy->setPosition({ 3, -2 });
	_enemy->setSortingOrder(70);
	_enemy->layer = Engine::BoundingBox::ENEMY;
}

void Platformer::update(float deltaTime)
{
	const float moveSpeed = 3.f;
	glm::vec2 offset = moveSpeed * deltaTime * _input.getMoveDirection();

	_player->move(offset);
}

void Platformer::onExit()
{
	Application::onExit();
}
