#pragma once
#include <Core.h>

namespace Engine
{
	class ENGINE_API Input
	{
	public:
		~Input() = default;
		Input();

		enum KeyCode
		{
			W, A, S, D,
			F, R, E, Q, C, Z, X,
			SPACE,
			RETURN,
			BACKSPACE,

			KEYS_AMOUNT
		};

		enum MouseButton
		{
			LEFT, RIGHT, MIDDLE,

			BUTTONS_AMOUNT
		};

		glm::vec2 getMoveDirection() const;
		bool isJump() const;

		bool keyPressed(KeyCode key) const { return _keysPressed[key]; }
		bool keyHold(KeyCode key) const { return _keysHold[key]; }
		bool keyReleased(KeyCode key) const { return _keysReleased[key]; }

		bool mouseButtonPressed(MouseButton button) const { return _mouseButtonsPressed[button]; }
		bool mouseButtonHold(MouseButton button) const { return _mouseButtonsHold[button]; }
		bool mouseButtonReleased(MouseButton button) const { return _mouseButtonsReleased[button]; }

		glm::vec2 mouseCoords() const;

		void handleEvents();

	private:

		//Mapping between SDL key codes and custom ones
		unsigned int mapping[323];

		bool _keysPressed[KEYS_AMOUNT];
		bool _keysHold[KEYS_AMOUNT];
		bool _keysReleased[KEYS_AMOUNT];

		bool _mouseButtonsPressed[BUTTONS_AMOUNT];
		bool _mouseButtonsHold[BUTTONS_AMOUNT];
		bool _mouseButtonsReleased[BUTTONS_AMOUNT];

		void peekInputEvents();
		void handleKeyboardInput(SDL_Event* events, int count);
		void handleMouseInput(SDL_Event* events, int count);

		void resetKeys();
		void resetMouseButtons();

		void initMapping();
		void initHoldings();
	};
}
