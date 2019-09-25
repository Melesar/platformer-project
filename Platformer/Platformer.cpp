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

	_sprite = createSprite();
}

void Platformer::update(float deltaTime)
{
		
}

void Platformer::onExit()
{
	destroySprite(_sprite);
}
