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

	Engine::Sprite* playerSprite = createSprite(Engine::TEX_ELLIOT, 500);
	_player = std::make_unique<Player>(playerSprite, _input, _raycaster);

	Engine::Sprite* ground = createSprite(Engine::TEX_PLATFORM, 128);
	ground->setLayer(Engine::BoundingBox::PLATFORM);
	ground->setPosition({ 0, -4 });
	ground->setSize({ 8, 1 });

	Engine::Sprite* box = createSprite(Engine::TEX_PLATFORM, 128);
	box->setLayer(Engine::BoundingBox::PLATFORM);
	box->setPosition({ 3, -2 });
	box->setSortingOrder(1);
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
