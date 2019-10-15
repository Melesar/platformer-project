#pragma once
#include "Core.h"
#include "Time.h"
#include "Rendering/Sprite.h"
#include "Input/Input.h"
#include "Data/ResourceRepository.h"
#include "Physics/Raycaster.h"
#include "GUI.h"


namespace Engine
{
	class Renderer;

	class ENGINE_API Application
	{
	public: 
		void run();
		void run(int argc, char** argv);

		bool isRunning() const;

		Application();
		virtual ~Application() = default;

		static Sprite* createSprite();
		static Sprite* createSprite(TextureId id);
		static Sprite* createSprite(TextureId id, int ppu);

		static void destroySprite(Sprite* sprite);

	protected:

		void stop();
		virtual void setup();
		virtual void update(float deltaTime) = 0;
		virtual void onExit();

	private:

		void initSDL();
		void update();

		void handleEvents();
		void updatePendingSprites();

		static void removeSprite(std::vector<Sprite*>& vector, Sprite* s);

	protected:

		std::string _title;
		bool _isFullscreen = false;
		float _worldHeight = 5.f;
		
		Renderer* _renderer = nullptr;
		Input _input;
		ResourcesRepository _resources;
		Raycaster _raycaster;

		static Application* _current;
		
	private:
		
		SDL_Window* _window = nullptr;
		SDL_GLContext _context;
		Time _time = {};
		bool _isRunning = false;

		GUI* _gui;
		
		std::vector<Sprite*> _sprites;
		std::vector<Sprite*> _pendingSprites;
	};
}
