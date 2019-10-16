#include "GameOverWidget.h"
#include "Application.h"
#include "Input/Input.h"
#include "Rendering/Renderer.h"

Platformer::GameOverWidget::GameOverWidget(const Engine::Renderer& renderer, const Engine::Input& input) :
	_renderer(renderer),
	_input(input)
{
	_text = Application::createSprite(Engine::TEX_GAME_OVER, 102);
	_text->setPosition({ 0, 3 });
	_text->setSortingOrder(1000);
	
	_button = Application::createSprite(Engine::TEX_BUTTON_RESTART, 100);
	_button->setPosition({ 0, -2 });
	_button->setSortingOrder(1000);
}

Platformer::GameOverWidget::~GameOverWidget()
{
	Application::destroySprite(_text);
	Application::destroySprite(_button);
}

bool Platformer::GameOverWidget::isButtonPressed() const
{
	if (!_input.mouseButtonPressed(Engine::Input::LEFT))
	{
		return false;
	}

	glm::vec2 mousePos = _renderer.screenToWorldPos(_input.mouseCoords());
	return _button->getBoundingBox().isPointInside(mousePos);
}