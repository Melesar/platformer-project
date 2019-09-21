#include "Core.h"
#include "Application.h"

void Engine::Application::Run()
{
	_isRunning = true;

	Setup();
	while (_isRunning)
	{
		Update();
	}

	OnExit();
}

void Engine::Application::Run(int argc, char** argv)
{
	Run();
}

bool Engine::Application::IsRunning()
{
	return _isRunning;
}

void Engine::Application::InitSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
}

void Engine::Application::Stop()
{
	_isRunning = false;
}

void Engine::Application::Setup()
{
	InitSDL();
}

void Engine::Application::Update()
{
	HandleEvents();

	_time.Update();
	Update(_time.delta);
}


void Engine::Application::HandleEvents()
{
	SDL_Event e[1];
	int count = SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT);
	_isRunning = count == 0;

	SDL_PumpEvents();
}

void Engine::Application::OnExit()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}


