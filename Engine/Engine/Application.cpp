#include "Core.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

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
	SDL_DisplayMode dm;
	if (!_isFullscreen || SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		dm.w = 800;
		dm.h = 600;
	}
	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, SDL_WINDOW_OPENGL);
}

void Engine::Application::stop()
{
	_isRunning = false;
}

void Engine::Application::setup()
{
	initSDL();
	
	_renderer = new Renderer(_window);
}

void Engine::Application::update()
{
	handleEvents();

	update(_time.delta());
	_time.update();

	_renderer->render();
}


void Engine::Application::handleEvents()
{
	SDL_Event e[1];
	int count = SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT);
	_isRunning = count == 0;

	_input.handleEvents();
}

void Engine::Application::onExit()
{
	delete _renderer;

	SDL_DestroyWindow(_window);
	SDL_Quit();
}

Engine::Sprite* Engine::Application::createSprite()
{
	std::shared_ptr<Texture> elliot = std::make_shared<Texture>("../res/Elliot.png", ELLIOT);
	//const auto s = new Sprite();
	const auto s = new Sprite(elliot, 500, 500);
	_renderer->submitForRendering(s);
	
	return s;
}

void Engine::Application::destroySprite(Sprite* sprite)
{
	_renderer->removeFromRenderingPool(sprite);
	delete sprite;
}


