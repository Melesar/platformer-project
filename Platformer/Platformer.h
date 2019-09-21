#pragma once
#include <Application.h>

class Platformer : public Engine::Application
{
public:

	Platformer();
	~Platformer() = default;
	
protected:

	void Setup() override;
	void Update(float deltaTime) override;
};

