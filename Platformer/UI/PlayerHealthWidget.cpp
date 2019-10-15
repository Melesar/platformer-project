#include "PlayerHealthWidget.h"
#include "Player/Player.h"
#include "Rendering/Sprite.h"
#include "Application.h"

void Platformer::PlayerHealthWidget::update()
{
	if (_player->isDamaged())
	{
		Engine::Sprite* heart = _hearts[_player->getLives()];
		glm::vec2 heartPosition = heart->getPosition();
		
		Application::destroySprite(heart);

		heart = Application::createSprite(Engine::TEX_HEART_EMPTY, 600);
		heart->setPosition(heartPosition);
		heart->setSortingOrder(1000);

		_hearts[_player->getLives()] = heart;
	}
}

Platformer::PlayerHealthWidget::PlayerHealthWidget(glm::vec2 position, Player* player) : _player(player)
{
	const int playerLives = _player->getMaxLives();
	_hearts.resize(playerLives);

	const float panelWidth = playerLives * _heartSize + (playerLives - 1) * _padding;
	glm::vec2 heartPosition = {position.x - (panelWidth + _heartSize) * 0.5, position.y};
	for (int i = 0; i < playerLives; ++i)
	{
		Engine::Sprite* heart = Application::createSprite(Engine::TEX_HEART, 600);
		heart->setPosition(heartPosition);
		heart->setSortingOrder(1000);

		_hearts[i] = heart;
		heartPosition.x += _heartSize + _padding;
	}
}
