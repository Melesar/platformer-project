#include "Platformer.h"
#include <Rendering/Renderer.h>

Platformer::Platformer()
{
	_title = "Platformer project";
}


void Platformer::setup()
{
	Application::setup();
	_renderer->setBackgroundColor({ 1.f, 0.f, 0.f, 1.f });
}

void Platformer::update(float deltaTime)
{
	
}
