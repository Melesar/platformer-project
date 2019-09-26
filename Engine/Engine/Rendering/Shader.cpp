#include "Core.h"
#include "Shader.h"
#include "Data/Color.h"

Engine::Shader::Shader(const std::string& fileName)
{
	_program = glCreateProgram();

	_shaders[0] = createShader(loadShader("../res/" + fileName + "Vertex.shader"), GL_VERTEX_SHADER);
	_shaders[1] = createShader(loadShader("../res/" + fileName + "Fragment.shader"), GL_FRAGMENT_SHADER);

	for (size_t i = 0; i < ShadersAmount; i++)
	{
		glAttachShader(_program, _shaders[i]);
	}

	glBindAttribLocation(_program, 0, "position");
	glBindAttribLocation(_program, 1, "texCoord");

	glLinkProgram(_program);
	checkForShaderErrors(_program, GL_LINK_STATUS, true, "Error linking program: ");

	glValidateProgram(_program);
	checkForShaderErrors(_program, GL_VALIDATE_STATUS, true, "Program is invalid: ");

	_uniforms[TRANSFORM_U] = glGetUniformLocation(_program, "transform");
	_uniforms[DIFFUSE_U]   = glGetUniformLocation(_program, "diffuse");
	_uniforms[COLOR_U]     = glGetUniformLocation(_program, "color");
}


Engine::Shader::~Shader()
{
	for (size_t i = 0; i < ShadersAmount; i++)
	{
		glDetachShader(_program, _shaders[i]);
		glDeleteShader(_shaders[i]);
	}

	glDeleteProgram(_program);
}

void Engine::Shader::bind() const
{
	glUseProgram(_program);
}

void Engine::Shader::setTransform(const glm::mat3x3& transform)
{
	glUniformMatrix3fv(_uniforms[TRANSFORM_U], 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&transform));
}

void Engine::Shader::setDiffuse(int textureIndex)
{
	glUniform1i(_uniforms[DIFFUSE_U], textureIndex);
}

void Engine::Shader::setColor(const Color& color)
{
	glUniform4fv(_uniforms[COLOR_U], 1, reinterpret_cast<const GLfloat*>(&color));
}


std::string Engine::Shader::loadShader(const std::string& fileName) const
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;
	if (file.is_open())
	{
		while(file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}else
	{
		std::cerr << "Unable to load shader " << fileName << std::endl;
	}

	return output;
}

void Engine::Shader::checkForShaderErrors(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof error, nullptr, error);
		else
			glGetShaderInfoLog(shader, sizeof error, nullptr, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Engine::Shader::createShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cerr << "Error: Shader creation failed" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	//Присвоение текста шейдера
	glShaderSource(shader, 1 /*количество шейдеров*/, shaderSourceStrings, shaderSourceStringLengths);

	//Компиляция шейдера
	glCompileShader(shader);

	checkForShaderErrors(shader, GL_COMPILE_STATUS, false, "Error: shader compilation failed: ");

	return shader;
}