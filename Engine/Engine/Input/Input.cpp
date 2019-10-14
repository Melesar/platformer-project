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

void Engine::Input::handleEvent(const SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
	{
		int keycode = e->key.keysym.sym;
		if (keycode < MAPPING_SIZE)
		{
			if (e->type == SDL_KEYDOWN)
			{
				_keysPressed[mapping[keycode]] = true;
				_keysHold[mapping[keycode]] = true;
			}
			else if (e->type == SDL_KEYUP)
			{
				_keysHold[mapping[keycode]] = false;
				_keysReleased[mapping[keycode]] = true;
			}
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		MouseButton button;
		switch (e->button.button) {
		case SDL_BUTTON_LEFT:
			button = LEFT;
			break;
		case SDL_BUTTON_RIGHT:
			button = RIGHT;
			break;
		case SDL_BUTTON_MIDDLE:
			button = MIDDLE;
			break;
		default:
			return;
		}

		if (e->type == SDL_MOUSEBUTTONDOWN) {
			_mouseButtonsPressed[button] = true;
			_mouseButtonsHold[button] = true;
		}
		else if (e->type == SDL_MOUSEBUTTONUP) {
			_mouseButtonsReleased[button] = true;
			_mouseButtonsHold[button] = false;
		}
	}

	
}

void Engine::Input::reset()
{
	resetKeys();
	resetMouseButtons();
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

