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

	_player = createSprite(Engine::TEX_ELLIOT, 500, 500);
	_player->setSortingOrder(50);
	_player->layer = Engine::BoundingBox::PLAYER;

	Engine::Sprite* enemy1 = createSprite(Engine::TEX_ELLIOT, 600, 600);
	enemy1->setPosition({ -3, 2 });
	enemy1->setSortingOrder(10);
	enemy1->layer = Engine::BoundingBox::ENEMY;

	_enemy = createSprite(Engine::TEX_ELLIOT, 700, 700);
	_enemy->setPosition({ 3, -2 });
	_enemy->setSortingOrder(70);
	_enemy->layer = Engine::BoundingBox::ENEMY;
}

void Platformer::update(float deltaTime)
{
	const float moveSpeed = 3.f;
	glm::vec2 offset = moveSpeed * deltaTime * _input.getMoveDirection();

	_player->move(offset);
	
	Engine::Ray ray(_player->getPosition(), glm::vec2(offset.x, 0));
	Engine::Intersection i{};
	if (_raycaster.raycast(ray, 3., Engine::BoundingBox::PLATFORM, i))
	{
		std::cout << "Intersection at distance " << i.distance << std::endl;
	}
}

void Platformer::onExit()
{
	Application::onExit();
}
