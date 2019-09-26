#include <Core.h>
#include "Input.h"

glm::vec2 Engine::Input::getMoveDirection() const
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


bool Engine::Input::isJump() const
{
	return keyPressed(SPACE);
}


glm::vec2 Engine::Input::mouseCoords() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	return glm::vec2(x, y);
}

void Engine::Input::handleEvents()
{
	resetKeys();
	resetMouseButtons();

	peekInputEvents();
}

void Engine::Input::peekInputEvents()
{
	SDL_Event mouseEvents[2];
	SDL_Event keyboardEvents[2];

	SDL_PumpEvents();
	int mouseCount = SDL_PeepEvents(mouseEvents, 2, SDL_GETEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP);
	int keyboardCount = SDL_PeepEvents(keyboardEvents, 2, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYUP);

	handleKeyboardInput(keyboardEvents, keyboardCount);
	handleMouseInput(mouseEvents, mouseCount);
}

void Engine::Input::handleKeyboardInput(SDL_Event* events, int count)
{
	for (int i = 0; i < count; i++) {
		if (events[i].type == SDL_KEYDOWN) {
			_keysPressed[mapping[events[i].key.keysym.sym]] = true;
			_keysHold[mapping[events[i].key.keysym.sym]] = true;
		}
		else if (events[i].type == SDL_KEYUP) {
			_keysHold[mapping[events[i].key.keysym.sym]] = false;
			_keysReleased[mapping[events[i].key.keysym.sym]] = true;
		}
	}
}

void Engine::Input::handleMouseInput(SDL_Event* events, int count)
{
	for (int i = 0; i < count; ++i) {
		MouseButton button;
		switch (events[i].button.button) {
			case SDL_BUTTON_LEFT:
				button = LEFT;
				break;
			case SDL_BUTTON_RIGHT:
				button = RIGHT;
				break;
			case SDL_BUTTON_MIDDLE:
				button = MIDDLE;
				break;
		}

		if (events[i].type == SDL_MOUSEBUTTONDOWN) {
			_mouseButtonsPressed[button] = true;
			_mouseButtonsHold[button] = true;
		}
		else if (events[i].type == SDL_MOUSEBUTTONUP) {
			_mouseButtonsReleased[button] = true;
			_mouseButtonsHold[button] = false;
		}
	}
}

void Engine::Input::resetKeys()
{
	for (int i = 0; i < KEYS_AMOUNT; ++i) {
		_keysReleased[i] = false;
		_keysPressed[i] = false;
	}
}

void Engine::Input::resetMouseButtons()
{
	for (int i = 0; i < BUTTONS_AMOUNT; ++i) {
		_mouseButtonsPressed[i] = false;
		_mouseButtonsReleased[i] = false;
	}
}

void Engine::Input::initMapping()
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

	mapping[SDLK_SPACE] = SPACE;
	mapping[SDLK_RETURN] = RETURN;
	mapping[SDLK_BACKSPACE] = BACKSPACE;
}

void Engine::Input::initHoldings()
{
	for (int i = 0; i < KEYS_AMOUNT; ++i) {
		_keysHold[i] = false;
	}

	for (int i = 0; i < BUTTONS_AMOUNT; ++i) {
		_mouseButtonsHold[i] = false;
	}
}

Engine::Input::Input()
{
	initMapping();
	initHoldings();

	resetKeys();
	resetMouseButtons();
}

