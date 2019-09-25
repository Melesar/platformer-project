#include <Core.h>
#include "Input.h"

glm::vec2 Input::getMoveDirection() const
{
	glm::vec2 dir(0.f);
	
	if (keyHold(W))
	{
		dir.y = 1;
	}
	else if (keyHold(S))
	{
		dir.y = -1;
	}

	if (keyHold(D))
	{
		dir.x = 1;
	}
	else if (keyHold(A))
	{
		dir.x = -1;
	}

	return dir;
}


bool Input::isJump() const
{
	return keyPressed(Space);
}


glm::vec2 Input::mouseCoords() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	return glm::vec2(x, y);
}

void Input::handleEvents()
{
	resetKeys();
	resetMouseButtons();

	peekInputEvents();
}

void Input::peekInputEvents()
{
	SDL_Event mouseEvents[2];
	SDL_Event keyboardEvents[2];

	SDL_PumpEvents();
	int mouseCount = SDL_PeepEvents(mouseEvents, 2, SDL_GETEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP);
	int keyboardCount = SDL_PeepEvents(keyboardEvents, 2, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYUP);

	handleKeyboardInput(keyboardEvents, keyboardCount);
	handleMouseInput(mouseEvents, mouseCount);
}

void Input::handleKeyboardInput(SDL_Event* events, int count)
{
	for (int i = 0; i < count; i++) {
		if (events[i].type == SDL_KEYDOWN) {
			keysPressed[mapping[events[i].key.keysym.sym]] = true;
			keysHold[mapping[events[i].key.keysym.sym]] = true;
		}
		else if (events[i].type == SDL_KEYUP) {
			keysHold[mapping[events[i].key.keysym.sym]] = false;
			keysReleased[mapping[events[i].key.keysym.sym]] = true;
		}
	}
}

void Input::handleMouseInput(SDL_Event* events, int count)
{
	for (int i = 0; i < count; ++i) {
		MouseButton button;
		switch (events[i].button.button) {
			case SDL_BUTTON_LEFT:
				button = Left;
				break;
			case SDL_BUTTON_RIGHT:
				button = Right;
				break;
			case SDL_BUTTON_MIDDLE:
				button = Middle;
				break;
		}

		if (events[i].type == SDL_MOUSEBUTTONDOWN) {
			mouseButtonsPressed[button] = true;
			mouseButtonsHold[button] = true;
		}
		else if (events[i].type == SDL_MOUSEBUTTONUP) {
			mouseButtonsReleased[button] = true;
			mouseButtonsHold[button] = false;
		}
	}
}

void Input::resetKeys()
{
	for (int i = 0; i < KEYS_AMOUNT; ++i) {
		keysReleased[i] = false;
		keysPressed[i] = false;
	}
}

void Input::resetMouseButtons()
{
	for (int i = 0; i < BUTTONS_AMOUNT; ++i) {
		mouseButtonsPressed[i] = false;
		mouseButtonsReleased[i] = false;
	}
}

void Input::initMapping()
{
	mapping[SDLK_w] = W;
	mapping[SDLK_a] = A;
	mapping[SDLK_s] = S;
	mapping[SDLK_d] = D;

	mapping[SDLK_f] = F;
	mapping[SDLK_q] = Q;
	mapping[SDLK_e] = E;
	mapping[SDLK_r] = R;
	mapping[SDLK_z] = Z;
	mapping[SDLK_x] = X;
	mapping[SDLK_c] = C;

	mapping[SDLK_SPACE] = Space;
	mapping[SDLK_RETURN] = Return;
	mapping[SDLK_BACKSPACE] = Backspace;
}

void Input::initHoldings()
{
	for (int i = 0; i < KEYS_AMOUNT; ++i) {
		keysHold[i] = false;
	}

	for (int i = 0; i < BUTTONS_AMOUNT; ++i) {
		mouseButtonsHold[i] = false;
	}
}

Input::Input()
{
	initMapping();
	initHoldings();

	resetKeys();
	resetMouseButtons();
}

