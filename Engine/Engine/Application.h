#pragma once
#include "Core.h"
#include "Time.h"
#include "Rendering/Sprite.h"


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
		Sprite* createSprite(int pixelsPerUnit);

		void destroySprite(Sprite* sprite);

	private:

		void initSDL();
		void update();
		void setScreenSize() const;

		void handleEvents();

	protected:

		std::string _title;
		Renderer* _renderer = nullptr;

	private:

		SDL_Window* _window = nullptr;
		Time _time = {};
		bool _isRunning = false;
	};
}
