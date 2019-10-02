#pragma once
#include <Core.h>
#include <Data/Color.h>
#include <Rendering/IRenderable.h>
#include "Physics/BoundingBox.h"
#include "Physics/IPhysicsBody.h"
#include "Rendering/Texture.h"

namespace Engine
{
	class Shader;
	
	class ENGINE_API Sprite final : public IRenderable, public IPhysicsBody
	{
	public:
		void render() const override;
		void setViewMatrix(const glm::mat3x3& matrix) override;
		int sortingOrder() const override;
		const BoundingBox& getBoundingBox() const override;
		
		void move(const glm::vec2& offset);
		void rotate(float deltaAngle);
		void resetUVs();
		void flipX(bool flip);
		
		glm::vec2 getPosition() const;
		float getScale() const;
		float getRotation() const;
		
		void setPosition(const glm::vec2& position);
		void setRotation(float rotation);
		void setScale(float scale);
		void setSortingOrder(int order);
		void setSize(glm::vec2 size);
		void setColor(const Color& color);
		void setLayer(BoundingBox::Layer layer);
		
		Sprite(std::shared_ptr<Shader> shader);
		Sprite(std::shared_ptr<Shader> shader, int ppu);
		Sprite(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, int ppu);
		explicit Sprite(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
		
		virtual ~Sprite();

	private:

		void updateTransform();
		void bindMesh();
		void rebuildMesh();
		void updateBoundingBox();
		void drawDebug() const;

	private:
		int _ppu;

		std::shared_ptr<Texture> _texture = nullptr;

		glm::vec2 _position;
		float _rotation;
		float _scale;
		int _sortingOrder = 0;

		const glm::vec2 _defaultSize = {
			_texture != nullptr ? static_cast<float>(_texture->width()) / _ppu : _ppu * 0.01f,
			_texture != nullptr ? static_cast<float>(_texture->height()) / _ppu : _ppu * 0.01f,
		};
		const glm::vec2 _invDefaultSize = 1.f / _defaultSize;
		
		glm::vec2 _size = _defaultSize;

		glm::mat3x3 _modelMatrix;
		glm::mat3x3 _viewMatrix{};
		glm::mat3x3 _transformMatrix;

		BoundingBox _bb;

		glm::vec2 _vertices[4]{};
		glm::vec2 _uvs[4]{};
		int _indices[6]{};

		enum
		{
			BUFFER_VERTICES,
			BUFFER_UVS,
			BUFFER_INDICES,

			NUM_BUFFERS,
		};

		GLuint _vao{};
		GLuint _vbo[NUM_BUFFERS]{};

		std::shared_ptr<Shader> _shader;
		Color _color = {1, 1, 1, 1};
	};
}


