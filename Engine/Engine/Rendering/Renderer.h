#pragma once
#include "Renderer.h"
#include "Data/Color.h"

namespace Engine
{
	class ENGINE_API Renderer
	{
	public:

		void Render();
		void SetBackgroundColor(Color color);

		Renderer(SDL_Window* window);
		~Renderer();

	private:

		Color _backgroundColor;

		SDL_Window* _window;
		SDL_GLContext _context;
		SDL_Surface* _windowSurface;
	};
}