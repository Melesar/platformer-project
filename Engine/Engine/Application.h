#pragma once
#include "Core.h"
#include "Time.h"
#include "Rendering/Sprite.h"
#include "Input/Input.h"
#include "Data/ResourceRepository.h"
#include "Physics/Raycaster.h"


namespace Engine
{
	class Renderer;

	class ENGINE_API Application
	{
	public: 
		void run();
		void run(int argc, char** argv);

		bool isRunning() const;

		virtual ~Application() = default;

	protected:

		void stop();
		virtual void setup();
		virtual void update(float deltaTime) = 0;
		virtual void onExit();

		Sprite* createSprite();
		Sprite* createSprite(TextureId id);
		Sprite* createSprite(TextureId id, int ppuHorizontal, int ppuVertical);

		void destroySprite(Sprite* sprite);

	private:

		void initSDL();
		void update();

		void handleEvents();
		void updatePendingSprites();

	protected:

		std::string _title;
		bool _isFullscreen = false;
		
		Renderer* _renderer = nullptr;
		Input _input;
		ResourcesRepository _resources;
		Raycaster _raycaster;

	private:

		SDL_Window* _window = nullptr;
		Time _time = {};
		bool _isRunning = false;

		std::vector<Sprite*> _sprites;
		std::vector<Sprite*> _pendingSprites;
	};
}
