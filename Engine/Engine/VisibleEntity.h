#pragma once
#include "Entity.h"
#include "Application.h"


namespace Engine
{
	class Sprite;
	
	class VisibleEntity : public Entity
	{
	public:

		Sprite* getSprite() const { return _sprite; }

		virtual ~VisibleEntity() { Application::destroySprite(_sprite); }
		
	protected:
		VisibleEntity(Sprite* sprite) : _sprite(sprite) {}

		Sprite* _sprite;
	};
}
