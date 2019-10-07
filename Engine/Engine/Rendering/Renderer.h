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

		float worldWidth() const;
		float worldHeight() const;
		float aspect() const;
		
		glm::vec2 worldToScreenPos(glm::vec2 worldPos) const;
		glm::vec2 screenToWorldPos(glm::vec2 screenPos) const;

		explicit Renderer(SDL_Window* window, float worldHeight);
		
		Renderer(const Renderer& other) = delete;
		Renderer& operator = (const Renderer& other) = delete;
		
		~Renderer();

	private:

		void setOutputSize(int screenWidth, int screenHeight, float halfWorldHeight);

	private:

		float _aspect;
		float _worldHeight, _worldWidth;
		int _outputWidth, _outputHeight;
		int _renderablesCount = 0;

		glm::mat3x3 _viewMatrix;
		
		Color _backgroundColor;
		SDL_Window* _window;
		SDL_GLContext _context;
		SDL_Surface* _windowSurface;

		std::set<IRenderable*> _renderablesSet;
		std::list<IRenderable*> _renderablesList;
	};
}