#include "Platformer.h"
#include <Rendering/Renderer.h>

Platformer::Platformer()
{
	_title = "Platformer project";
}


void Platformer::Setup()
{
	Engine::Application::Setup();
	_renderer->SetBackgroundColor({ 1.f, 0.f, 0.f, 1.f });
}

void Platformer::Update(float deltaTime)
{
	
}
