#pragma once
#include <Engine/Application.h>

class Platformer : public Engine::Application
{
public:

	Platformer();
	~Platformer() = default;
	
protected:

	void Update(float deltaTime) override;
};

