#pragma once
#include "Player/Player.h"
#include "../Engine/Engine/Application.h"


namespace Platformer
{
	class Application final : public Engine::Application
	{
	public:

		Application();
		~Application() = default;

	protected:

		void setup() override;
		void update(float deltaTime) override;
		void onExit() override;

	private:

		std::unique_ptr<Player> _player;
	};
}



