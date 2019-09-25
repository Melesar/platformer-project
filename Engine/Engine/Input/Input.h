#pragma once
#include <Core.h>

class ENGINE_API Input
{
public:
	~Input() = default;
	Input();

	enum KeyCode
	{
		W, A, S, D,
		F, R, E, Q, C, Z, X,
		Space,
		Return,
		Backspace,

		KEYS_AMOUNT
	};

	enum MouseButton
	{
		Left, Right, Middle,

		BUTTONS_AMOUNT
	};

	glm::vec2 getMoveDirection() const;
	bool isJump() const;

	bool keyPressed(KeyCode key) const { return keysPressed[key]; }
	bool keyHold(KeyCode key) const { return keysHold[key]; }
	bool keyReleased(KeyCode key) const { return keysReleased[key]; }

	bool mouseButtonPressed(MouseButton button) const { return mouseButtonsPressed[button]; }
	bool mouseButtonHold(MouseButton button) const { return mouseButtonsHold[button]; }
	bool mouseButtonReleased(MouseButton button) const { return mouseButtonsReleased[button]; }

	glm::vec2 mouseCoords() const;
	
	void handleEvents();

private:

	//Mapping between SDL key codes and custom ones
	unsigned int mapping[323];

	bool keysPressed[KEYS_AMOUNT];
	bool keysHold[KEYS_AMOUNT];
	bool keysReleased[KEYS_AMOUNT];

	bool mouseButtonsPressed[BUTTONS_AMOUNT];
	bool mouseButtonsHold[BUTTONS_AMOUNT];
	bool mouseButtonsReleased[BUTTONS_AMOUNT];

	void peekInputEvents();
	void handleKeyboardInput(SDL_Event* events, int count);
	void handleMouseInput(SDL_Event* events, int count);

	void resetKeys();
	void resetMouseButtons();

	void initMapping();
	void initHoldings();
};