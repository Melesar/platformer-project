#pragma once
#include <Core.h>
#include <Rendering/IRenderable.h>

namespace Engine
{
	class Shader;
	
	class ENGINE_API Sprite : public IRenderable
	{
	public:
		void render() const override;
		void setViewMatrix(const glm::mat3x3& matrix) override;

		void move(const glm::vec2& offset);
		
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

		glm::mat3x3 updateTransformMatrix() const;
		void bindMesh();
		void rebuildMesh();
		
	private:
		int _pixelsPerUnit;

		glm::vec2 _position;
		float _rotation;
		float _scale;
		
		glm::mat3x3 _transformation;
		glm::mat3x3 _viewMatrix;

		glm::vec3 _vertices[4];
		glm::vec2 _uvs[4];
		int _indices[6];

		enum
		{
			BUFFER_VERTICES,
			BUFFER_UVS,
			BUFFER_INDICES,

			NUM_BUFFERS,
		};

		GLuint _vao;
		GLuint _vbo[NUM_BUFFERS];

		std::unique_ptr<Shader> _shader;

		const std::string _shaderName = "shader";
	};
}


