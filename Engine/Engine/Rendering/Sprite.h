#pragma once
#include <Core.h>
#include <Rendering/IRenderable.h>

namespace Engine
{
	class ENGINE_API Sprite : public IRenderable
	{
	public:
		void Render() const override;

		glm::vec2 getPosition() const;
		float getScale() const;
		float getRotation() const;
		
		void setPosition(const glm::vec2& position);
		void setRotation(float rotation);
		void setScale(float scale);

		Sprite();
		explicit Sprite(int pixelsPerUnit);
		~Sprite();

	private:

		glm::mat4x4 updateTransformMatrix() const;
		void rebuildMesh();
		
	private:
		int _pixelsPerUnit;

		glm::vec2 _position;
		float _rotation;
		float _scale;
		
		glm::mat4x4 _transformation;
	};
}


