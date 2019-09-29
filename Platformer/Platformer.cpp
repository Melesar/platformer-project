#include "Platformer.h"
#include <Rendering/Renderer.h>

Platformer::Platformer()
{
	_title = "Platformer project";
}

void Platformer::setup()
{
	Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	_player = createSprite(Engine::TEX_ELLIOT, 500, 500);
	_player->setSortingOrder(50);
	_player->layer = Engine::BoundingBox::PLAYER;

	Engine::Sprite* enemy1 = createSprite(Engine::TEX_ELLIOT, 600, 600);
	enemy1->setPosition({ -3, 2 });
	enemy1->setSortingOrder(10);
	enemy1->layer = Engine::BoundingBox::ENEMY;

	Engine::Sprite* enemy2 = createSprite(Engine::TEX_ELLIOT, 700, 700);
	enemy2->setPosition({ 3, -2 });
	enemy2->setSortingOrder(70);
	enemy2->layer = Engine::BoundingBox::ENEMY;
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
