#pragma once
#include "Entity.h"


namespace Engine
{
	class Sprite;
	
	class VisibleEntity : public Entity
	{
	public:

		VisibleEntity(Sprite* sprite) : _sprite(sprite) {}
		Sprite* getSprite() const { return _sprite; }
		
	protected:

		Sprite* _sprite;
	};
}
