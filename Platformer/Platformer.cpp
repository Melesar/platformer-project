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

	_player = createSprite();
	//_player->setColor({ 0.97, 0.57, 0.12, 1 });
}

void Platformer::update(float deltaTime)
{
	const float moveSpeed = 3.f;
	glm::vec2 offset = moveSpeed * deltaTime * _input.getMoveDirection();
	
	_player->move(offset);
}

void Platformer::onExit()
{
	destroySprite(_player);
	Application::onExit();
}
