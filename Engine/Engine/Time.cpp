#include "Core.h"
#include "Time.h"

void Engine::Time::Update()
{
	_lastTick = SDL_GetTicks();
}

float Engine::Time::delta()
{
	return static_cast<float>(SDL_GetTicks() - _lastTick) / 1000.0f;
}
