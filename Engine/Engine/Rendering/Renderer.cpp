#include "Core.h"
#include "Renderer.h"
#include "IRenderable.h"

void Engine::Renderer::submitForRendering(IRenderable* renderable)
{
	if (_renderablesSet.find(renderable) != _renderablesSet.end())
	{
		return;
	}

	renderable->setViewMatrix(_viewMatrix);
	
	_renderablesSet.emplace(renderable);
	bool inserted = false;
	for(auto i = _renderablesList.begin(); i != _renderablesList.end(); ++i)
	{
		if ((*i)->sortingOrder() > renderable->sortingOrder())
		{
			_renderablesList.insert(i, renderable);
			inserted = true;
			break;
		}
	}
	if (!inserted)
	{
		_renderablesList.push_back(renderable);
	}

	_renderablesCount += 1;
}

void Engine::Renderer::removeFromRenderingPool(IRenderable* renderable)
{
	auto iter = _renderablesSet.find(renderable);
	if (iter != _renderablesSet.end())
	{
		_renderablesSet.erase(iter);
		_renderablesList.remove(renderable);
		_renderablesCount -= 1;
	}
}

void Engine::Renderer::render()
{
	SDL_GL_SwapWindow(_window);
	
	glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto renderable : _renderablesList)
	{
		renderable->render();
	}
}

void Engine::Renderer::setBackgroundColor(Color color)
{
	_backgroundColor = color;
}

float Engine::Renderer::worldWidth() const
{
	return _worldWidth;
}

float Engine::Renderer::worldHeight() const
{
	return _worldHeight;
}

float Engine::Renderer::aspect() const
{
	return  _aspect;
}

void Engine::Renderer::setOutputSize(int screenWidth, int screenHeight, float worldHeight)
{
	_outputWidth = screenWidth;
	_outputHeight = screenHeight;

	_aspect = static_cast<float>(screenWidth) / screenHeight;

	_worldHeight = worldHeight;
	_worldWidth = _worldHeight * _aspect;

	_viewMatrix[0][0] = 2.f / _worldWidth;
	_viewMatrix[1][1] = 2.f / _worldHeight;
	_viewMatrix[2][2] = 1;
}

Engine::Renderer::Renderer(SDL_Window* window, float worldHeight) :
	_window(window),
	_context(SDL_GL_CreateContext(window)),
	_windowSurface(SDL_GetWindowSurface(window))
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int width, height;
	SDL_GetWindowSize(_window, &width, &height);
	setOutputSize(width, height, worldHeight);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	glewInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Engine::Renderer::~Renderer()
{
	SDL_FreeSurface(_windowSurface);
	SDL_GL_DeleteContext(_context);
	IMG_Quit();
}

glm::vec2 Engine::Renderer::worldToScreenPos(glm::vec2 worldPos) const
{
	const glm::vec2 clip = worldPos / glm::vec2(_worldWidth, _worldHeight);
	const glm::vec2 screenNormalized = (clip + glm::vec2(1, 1)) / 2.f;

	return screenNormalized * glm::vec2(_outputWidth, _outputHeight);
}

glm::vec2 Engine::Renderer::screenToWorldPos(glm::vec2 screenPos) const
{
	screenPos.x -= _outputWidth * 0.5f;
	screenPos.y = -(screenPos.y - _outputHeight * 0.5f);

	screenPos = screenPos / glm::vec2(_outputWidth, _outputHeight) * glm::vec2(_worldWidth, _worldHeight);

	return screenPos;
}
