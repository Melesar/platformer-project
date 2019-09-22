#include "Core.h"
#include "Sprite.h"

void Engine::Sprite::Render() const
{
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
}

Engine::Sprite::Sprite(int pixelsPerUnit) :
	_pixelsPerUnit(pixelsPerUnit),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_transformation(updateTransformMatrix())
{
	rebuildMesh();
}

Engine::Sprite::~Sprite()
{
}

glm::mat4x4 Engine::Sprite::updateTransformMatrix() const
{
	return glm::scale(glm::vec3(_scale, _scale, _scale)) *
		glm::rotate(_rotation, glm::vec3(0.f, 0.f, 1.f)) *
		glm::translate(glm::vec3(_position.x, _position.y, 1.f));
		
}

void Engine::Sprite::rebuildMesh()
{
	
}
