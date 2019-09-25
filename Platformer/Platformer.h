#pragma once
#include <Application.h>

class Platformer : public Engine::Application
{
public:

	Platformer();
	~Platformer() = default;
	
protected:

	void setup() override;
	void update(float deltaTime) override;
	void onExit() override;
	
private:

	Engine::Sprite* _player;
};

