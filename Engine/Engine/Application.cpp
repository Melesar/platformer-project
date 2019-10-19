#include "Core.h"
#include "Application.h"
#include "Rendering/Renderer.h"

Engine::Application* Engine::Application::_current;

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

Engine::Application::Application()
{
	_current = this;
}

Engine::Application::~Application()
{
	while(!_sprites.empty())
	{
		destroySprite(*(_sprites.end() - 1));
	}

	delete _renderer;
	delete _gui;

	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
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
	_context = SDL_GL_CreateContext(_window);
}

void Engine::Application::stop()
{
	_isRunning = false;
}

void Engine::Application::setup()
{
	initSDL();

    _renderer = new Renderer(_window, _worldHeight);
    _gui = new GUI(_window, _context);
	_resources.loadResources();
}

void Engine::Application::update()
{
	handleEvents();
	
	_gui->begin();
	update(_time.delta());
	_gui->end();
	
	_renderer->render();
}


void Engine::Application::handleEvents()
{
	_input.reset();
	
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			_isRunning = false;
		}

		_input.handleEvent(&e);
		_gui->handleEvent(&e);
	}
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
	
}

Engine::Sprite* Engine::Application::createSprite()
{
	std::shared_ptr<Shader> shader = _current->_resources.getShader(SHADER_SPRITE);
	const auto s = new Sprite(shader);
	_current->_pendingSprites.push_back(s);
	
	return s;
}

Engine::Sprite* Engine::Application::createSprite(TextureId id)
{
	std::shared_ptr<Shader> shader = _current->_resources.getShader(SHADER_SPRITE);
	std::shared_ptr<Texture> texture = _current->_resources.getTexture(id);
	const auto s = new Sprite(shader, texture);
	_current->_pendingSprites.push_back(s);

	return s;
}

Engine::Sprite* Engine::Application::createSprite(TextureId id, int ppu)
{
	std::shared_ptr<Shader> shader = _current->_resources.getShader(SHADER_SPRITE);
	std::shared_ptr<Texture> texture = _current->_resources.getTexture(id);
	const auto s = new Sprite(shader, texture, ppu);
	_current->_pendingSprites.push_back(s);

	return s;
}


void Engine::Application::destroySprite(Sprite* sprite)
{
	_current->_renderer->removeFromRenderingPool(sprite);
	_current->_raycaster.removeBoundingBox(*sprite);

	removeSprite(_current->_sprites, sprite);
	removeSprite(_current->_pendingSprites, sprite);
	
	delete sprite;
}

void Engine::Application::removeSprite(std::vector<Sprite*>& vector, Sprite* s)
{
	for (auto i = vector.begin(); i < vector.end(); ++i)
	{
		if (*i == s)
		{
			vector.erase(i);
			return;
		}
	}
}