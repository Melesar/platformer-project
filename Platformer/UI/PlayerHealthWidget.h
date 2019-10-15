#pragma once
#include <Core.h>

namespace Engine
{
	class Sprite;
}

namespace Platformer
{
	class Player;
		
	class PlayerHealthWidget
	{
	public:

		void update();

		PlayerHealthWidget(glm::vec2 position, Player* player);

	private:

		const float _padding = 0.3f;
		const float _heartSize = 0.5f;
		
		Player* _player;

		std::vector<Engine::Sprite*> _hearts;
	};
}


