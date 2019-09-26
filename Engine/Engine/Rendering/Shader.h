#pragma once
#include <Core.h>

namespace Engine
{
	struct Color;
	
	class ENGINE_API Shader
	{
	public:
		explicit Shader(const std::string& fileName);
		virtual ~Shader();

		void bind() const;
		void setTransform(const glm::mat3x3& transform);
		void setDiffuse(int textureIndex);
		void setColor(const Color& color);

	private:
		
		static unsigned const int ShadersAmount = 2;

		std::string loadShader(const std::string& fileName) const;
		void checkForShaderErrors(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
		GLuint createShader(const std::string& text, GLenum shaderType);

		enum
		{
			TRANSFORM_U,
			TRANSLATION_U,
			DIFFUSE_U,
			COLOR_U,

			NUM_UNIFORMS
		};

		GLuint _program;
		GLuint _shaders[ShadersAmount];
		GLuint _uniforms[NUM_UNIFORMS];
	};
}


