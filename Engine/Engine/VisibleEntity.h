#pragma once
#include "EngineEntity.h"
#include "Application.h"


namespace Engine
{
	class Sprite;
	
	class VisibleEntity : public Entity
	{
	public:

		Sprite* getSprite() const { return _sprite; }

		~VisibleEntity() override { Application::destroySprite(_sprite); }
		
	protected:
		explicit VisibleEntity(Sprite* sprite) : _sprite(sprite) {}

		Sprite* _sprite;
	};
}
