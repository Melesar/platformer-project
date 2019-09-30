#include "Core.h"
#include "Time.h"

void Engine::Time::update()
{
	const unsigned currentTicks = SDL_GetTicks();
	_delta = static_cast<float>(currentTicks - _lastTick) / 1000.0f;
	_lastTick = currentTicks;
}

void Engine::Time::init()
{
	_lastTick = SDL_GetTicks();
	_delta = 0.f;
}

float Engine::Time::delta() const
{
	return _delta;
}
