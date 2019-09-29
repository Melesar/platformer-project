#include "Application.h"
#include <Rendering/Renderer.h>
#include "Physics/Ray.h"

Platformer::Application::Application()
{
	_title = "Platformer project";
}

void Platformer::Application::setup()
{
	Engine::Application::setup();
	_renderer->setBackgroundColor({ 0.42, 0.77, 0.77, 1 });

	Engine::Sprite* playerSprite = createSprite(Engine::TEX_ELLIOT, 500, 500);
	_player = std::make_unique<Player>(playerSprite, _input, _raycaster);

	Engine::Sprite* platform = createSprite(Engine::TEX_PLATFORM, 300, 300);
	platform->setPosition({ 0, -4 });
	platform->setSize({ 8, 1 });
}

bool flip = false;

void Platformer::Application::update(float deltaTime)
{
	_player->update(deltaTime);
}

void Platformer::Application::onExit()
{
	Engine::Application::onExit();
}
