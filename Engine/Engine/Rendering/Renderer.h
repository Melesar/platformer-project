#pragma once
#include "Renderer.h"
#include "Data/Color.h"

namespace Engine
{
	class IRenderable;

	class ENGINE_API Renderer
	{
	public:

		void submitForRendering(IRenderable* renderable);
		void removeFromRenderingPool(IRenderable* renderable);
		
		void render();
		void setBackgroundColor(Color color);
		
		glm::vec2 worldToScreenPos(glm::vec2 worldPos) const;

		explicit Renderer(SDL_Window* window);
		
		Renderer(const Renderer& other) = delete;
		Renderer& operator = (const Renderer& other) = delete;
		
		~Renderer();

	private:

		void setOutputSize(int screenWidth, int screenHeight, float worldHeight);

	private:

		float _aspect;
		float _worldHeight, _worldWidth;
		int _outputWidth, _outputHeight;
		int _renderablesCount = 0;
		
		Color _backgroundColor;
		SDL_Window* _window;
		SDL_GLContext _context;
		SDL_Surface* _windowSurface;

		std::set<IRenderable*> _renderablesSet;
		std::vector<IRenderable*> _renderablesList;

		glm::mat2x2 _viewMatrix;
	};
}