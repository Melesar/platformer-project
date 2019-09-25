#include "Core.h"
#include "Sprite.h"
#include "Shader.h"

void Engine::Sprite::render() const
{
	_shader->bind();
	_shader->setTransform(_viewMatrix * _transformation);

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Engine::Sprite::setViewMatrix(const glm::mat3x3& matrix)
{
	_viewMatrix = matrix;
}

void Engine::Sprite::move(const glm::vec2& offset)
{
	setPosition(_position + offset);
}

glm::vec2 Engine::Sprite::getPosition() const
{
	return _position;
}

void Engine::Sprite::setPosition(const glm::vec2& position)
{
	_position = position;
	_transformation = updateTransformMatrix();
}

float Engine::Sprite::getRotation() const
{
	return _rotation;
}

void Engine::Sprite::setRotation(const float rotation)
{
	_rotation = rotation;
	_transformation = updateTransformMatrix();
}

float Engine::Sprite::getScale() const
{
	return _scale;
}

void Engine::Sprite::setScale(const float scale)
{
	_scale = scale;
	_transformation = updateTransformMatrix();
}

Engine::Sprite::Sprite() :
	_pixelsPerUnit(100),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_transformation(updateTransformMatrix())
{
	rebuildMesh();
	bindMesh();

	_shader = std::make_unique<Shader>(_shaderName);
}

Engine::Sprite::Sprite(int pixelsPerUnit) :
	_pixelsPerUnit(pixelsPerUnit),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_transformation(updateTransformMatrix())
{
	rebuildMesh();
	bindMesh();

	_shader = std::make_unique<Shader>(_shaderName);
}

Engine::Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &_vao);
}

glm::mat3x3 Engine::Sprite::updateTransformMatrix() const
{
	glm::mat3x3 transformation;
	transformation[0][0] = _scale * glm::cos(glm::radians(_rotation));
	transformation[0][1] = -glm::sin(glm::radians(_rotation));
	transformation[0][2] = 0;
	transformation[1][0] = -transformation[0][1];
	transformation[1][1] = transformation[0][0];
	transformation[1][2] = 0;
	transformation[2][0] = _position.x;
	transformation[2][1] = _position.y;
	transformation[2][2] = 1;
	
	return transformation;
}

void Engine::Sprite::bindMesh()
{
	glGenBuffers(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(NUM_BUFFERS, _vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BUFFER_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof _vertices[0], _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BUFFER_UVS]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof _uvs[0], _uvs, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[BUFFER_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof _indices[0], _indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

void Engine::Sprite::rebuildMesh()
{
	_vertices[0] = glm::vec3(-1, -1, 0);
	_vertices[1] = glm::vec3(-1, 1, 0);
	_vertices[2] = glm::vec3(1, 1, 0);
	_vertices[3] = glm::vec3(1, -1, 0);

	_uvs[0] = glm::vec2(0, 0);
	_uvs[1] = glm::vec2(0, 1);
	_uvs[2] = glm::vec2(1, 1);
	_uvs[3] = glm::vec2(1, 0);

	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 2;
	
	_indices[3] = 2;
	_indices[4] = 3;
	_indices[5] = 0;
}
