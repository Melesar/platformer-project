#include "Core.h"
#include "Application.h"
#include "Rendering/Renderer.h"

void Engine::Application::run()
{
	_isRunning = true;

	setup();
	while (_isRunning)
	{
		update();
	}

	onExit();
}

void Engine::Application::run(int argc, char** argv)
{
	run();
}

bool Engine::Application::isRunning() const
{
	return _isRunning;
}

void Engine::Application::initSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
}

void Engine::Application::stop()
{
	_isRunning = false;
}

void Engine::Application::setup()
{
	initSDL();
	
	_renderer = new Renderer(_window);
	
	setScreenSize();
}

void Engine::Application::update()
{
	handleEvents();

	_time.update();
	update(_time.delta());

	_renderer->render();
}

void Engine::Application::setScreenSize() const
{
	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		_renderer->setOutputSize(1, 1, 5.f);
	}
	else
	{
		_renderer->setOutputSize(dm.w, dm.h, 5.f);
	}
}


void Engine::Application::handleEvents()
{
	SDL_Event e[1];
	int count = SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT);
	_isRunning = count == 0;

	SDL_PumpEvents();
}

void Engine::Application::onExit()
{
	delete _renderer;

	SDL_DestroyWindow(_window);
	SDL_Quit();
}

Engine::Sprite* Engine::Application::createSprite()
{
	const auto s = new Sprite();
	_renderer->submitForRendering(s);
	
	return s;
}

Engine::Sprite* Engine::Application::createSprite(int pixelsPerUnit)
{
	const auto s = new Sprite(pixelsPerUnit);
	_renderer->submitForRendering(s);

	return s;
}

void Engine::Application::destroySprite(Sprite* sprite)
{
	_renderer->removeFromRenderingPool(sprite);
	delete sprite;
}


