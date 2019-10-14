#pragma once
#include "Core.h"

namespace Engine
{
	class GUI
	{
	public:

		GUI(SDL_Window* window, void* glContext);
		~GUI();

		void handleEvent(const SDL_Event* event);
		void begin() const;
		void end();

	private:

		SDL_Window* _window;
	};
}


