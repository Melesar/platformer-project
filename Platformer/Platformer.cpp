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

	Engine::Sprite* enemy1 = createSprite(Engine::TEX_ELLIOT, 600, 600);
	enemy1->setPosition({ -3, 2 });
	enemy1->setSortingOrder(10);

	Engine::Sprite* enemy2 = createSprite(Engine::TEX_ELLIOT, 700, 700);
	enemy2->setPosition({ 3, -2 });
	enemy2->setSortingOrder(70);
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
