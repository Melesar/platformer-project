#pragma once
#include <Core.h>
#include "Rendering/Sprite.h"

namespace Engine {
	class Input;
	class Renderer;
}

namespace Platformer
{
	class GameOverWidget
	{
	public:
		GameOverWidget(const Engine::Renderer& renderer, const Engine::Input& input);
		virtual ~GameOverWidget();

		bool isButtonPressed() const;
		
	private:
		const Engine::Renderer& _renderer;
		const Engine::Input& _input;

		bool _isButtonPressed = false;

		Engine::Sprite* _text;
		Engine::Sprite* _button;
	};
}


