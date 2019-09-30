#include "Core.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

void Engine::Application::run()
{
	_isRunning = true;

	setup();
	updatePendingSprites();

	_time.init();
	
	while (_isRunning)
	{
		update();
		updatePendingSprites();

		_time.update();
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
	_resources.loadResources();
}

void Engine::Application::update()
{
	handleEvents();

	update(_time.delta());

	_renderer->render();
}


void Engine::Application::handleEvents()
{
	SDL_Event e[1];
	int count = SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT);
	_isRunning = count == 0;

	_input.handleEvents();
}

void Engine::Application::updatePendingSprites()
{
	for (Sprite* sprite : _pendingSprites)
	{
		_renderer->submitForRendering(sprite);
		_raycaster.addBoundingBox(*sprite);
		_sprites.push_back(sprite);
	}

	_pendingSprites.erase(_pendingSprites.begin(), _pendingSprites.end());
}

void Engine::Application::onExit()
{
	for (Sprite* sprite : _sprites)
	{
		destroySprite(sprite);
	}
	
	delete _renderer;

	SDL_DestroyWindow(_window);
	SDL_Quit();
}

Engine::Sprite* Engine::Application::createSprite()
{
	std::shared_ptr<Shader> shader = _resources.getShader(SHADER_SPRITE);
	const auto s = new Sprite(shader);
	_pendingSprites.push_back(s);
	
	return s;
}

Engine::Sprite* Engine::Application::createSprite(TextureId id)
{
	std::shared_ptr<Shader> shader = _resources.getShader(SHADER_SPRITE);
	std::shared_ptr<Texture> texture = _resources.getTexture(id);
	const auto s = new Sprite(shader, texture);
	_pendingSprites.push_back(s);

	return s;
}

Engine::Sprite* Engine::Application::createSprite(TextureId id, int ppuHorizontal, int ppuVertical)
{
	std::shared_ptr<Shader> shader = _resources.getShader(SHADER_SPRITE);
	std::shared_ptr<Texture> texture = _resources.getTexture(id);
	const auto s = new Sprite(shader, texture, ppuVertical, ppuHorizontal);;
	_pendingSprites.push_back(s);

	return s;
}


void Engine::Application::destroySprite(Sprite* sprite)
{
	_renderer->removeFromRenderingPool(sprite);
	_raycaster.removeBoundingBox(*sprite);
	
	std::remove(_sprites.begin(), _sprites.end(), sprite);
	std::remove(_pendingSprites.begin(), _pendingSprites.end(), sprite);
	
	delete sprite;
}