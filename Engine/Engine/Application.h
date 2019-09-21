#pragma once
#include "Core.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public: 
		void Run();
		void Run(int argc, char** argv);

		bool IsRunning();

		virtual ~Application() = default;

	protected:

		void Stop();
		virtual void Setup();
		virtual void Update(float deltaTime) = 0;
		virtual void OnExit();

	private:

		void InitSDL();
		void Update();

		void HandleEvents();

	protected:

		std::string _title;

	private:

		SDL_Window* _window;
		bool _isRunning;
	};
}