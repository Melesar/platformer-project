#include "Core.h"
#include "Renderer.h"

void Engine::Renderer::render()
{
	SDL_GL_SwapWindow(_window);
	
	glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::Renderer::setBackgroundColor(Color color)
{
	_backgroundColor = color;
}

Engine::Renderer::Renderer(SDL_Window* window) :
	_window(window),
	_context(SDL_GL_CreateContext(window)),
	_windowSurface(SDL_GetWindowSurface(window))
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glewInit();
}


Engine::Renderer::~Renderer()
{
	SDL_FreeSurface(_windowSurface);
	SDL_GL_DeleteContext(_context);
}
