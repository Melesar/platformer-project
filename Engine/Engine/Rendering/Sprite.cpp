#include "Core.h"
#include "Sprite.h"
#include <utility>
#include "Shader.h"
#include "Texture.h"


void Engine::Sprite::render() const
{
	_shader->bind();
	if (_texture != nullptr)
	{
		_texture->bind();
		_shader->setDiffuse(_texture->id());
	}

	const glm::mat3x3 finalTransform = _viewMatrix * _transformation;
	_shader->setTransform(finalTransform);
	_shader->setColor(_color);

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Engine::Sprite::setViewMatrix(const glm::mat3x3& matrix)
{
	_viewMatrix = matrix;
}

int Engine::Sprite::sortingOrder() const
{
	return _sortingOrder;
}

void Engine::Sprite::move(const glm::vec2& offset)
{
	setPosition(_position + offset);
}

void Engine::Sprite::rotate(float deltaAngle)
{
	setRotation(_rotation + deltaAngle);
}

void Engine::Sprite::flipX(bool flip)
{
	float uv01 = flip ? _size.x : 0;
	float uv23 = flip ? 0 : _size.x;
	if (_uvs[0].x == uv01 && _uvs[2].x == uv23)
	{
		return;
	}

	_uvs[0].x = _uvs[1].x = uv01;
	_uvs[2].x = _uvs[3].x = uv23;
	
	resetUVs();
}

glm::vec2 Engine::Sprite::getPosition() const
{
	return _position;
}

void Engine::Sprite::setPosition(const glm::vec2& position)
{
	_position = position;
	updateTransform();
}

float Engine::Sprite::getRotation() const
{
	return _rotation;
}

void Engine::Sprite::setRotation(const float rotation)
{
	_rotation = rotation;
	updateTransform();
}

float Engine::Sprite::getScale() const
{
	return _scale;
}

void Engine::Sprite::setScale(const float scale)
{
	_scale = scale;
	updateTransform();
}

void Engine::Sprite::setSortingOrder(int order)
{
	_sortingOrder = order;
	updateTransform();
}

void Engine::Sprite::setSize(glm::vec2 size)
{
	_size = size;

	rebuildMesh();

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BUFFER_VERTICES]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof _vertices[0], _vertices);

	resetUVs();
}

void Engine::Sprite::setColor(const Color& color)
{
	_color = color;
}

Engine::Sprite::Sprite(std::shared_ptr<Shader> shader) :
	_ppuHorizontal(100),
	_ppuVertical(100),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_shader(std::move(shader))
{
	rebuildMesh();
	bindMesh();
	updateTransform();
}

Engine::Sprite::Sprite(std::shared_ptr<Shader> shader, int ppuVertical, int ppuHorizontal) :
	_ppuHorizontal(ppuHorizontal),
	_ppuVertical(ppuVertical),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_shader(std::move(shader))
{
	rebuildMesh();
	bindMesh();
	updateTransform();
}

Engine::Sprite::Sprite(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, int ppuVertical, int ppuHorizontal) :
	_ppuHorizontal(ppuHorizontal),
	_ppuVertical(ppuVertical),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_shader(std::move(shader)),
	_texture(std::move(texture))
{
	rebuildMesh();
	bindMesh();
	updateTransform();
}

Engine::Sprite::Sprite(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture) :
	_ppuHorizontal(100),
	_ppuVertical(100),
	_position(glm::vec2(0.f)),
	_rotation(0.f),
	_scale(1.f),
	_shader(std::move(shader)),
	_texture(std::move(texture))
{
	rebuildMesh();
	bindMesh();
	updateTransform();
}


Engine::Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &_vao);
}

void Engine::Sprite::updateTransform() 
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

	min = vec2(transformation * vec3(_vertices[0], 1));
	max = vec2(transformation * vec3(_vertices[2], 1));
	
	_transformation = transformation;
}

void Engine::Sprite::bindMesh()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(NUM_BUFFERS, _vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BUFFER_VERTICES]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof _vertices[0], _vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BUFFER_UVS]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof _uvs[0], _uvs, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[BUFFER_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof _indices[0], _indices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);

}

void Engine::Sprite::resetUVs()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BUFFER_UVS]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof _uvs[0], _uvs);
}

void Engine::Sprite::rebuildMesh()
{
	float extentX = _texture != nullptr ? static_cast<float>(_texture->width()) / _ppuHorizontal : _ppuHorizontal * 0.01f;
	float extentY = _texture != nullptr ? static_cast<float>(_texture->height()) / _ppuVertical : _ppuVertical * 0.01f;
	extentX *= _size.x * 0.5;
	extentY *= _size.y * 0.5;
	
	_vertices[0] = glm::vec2(-extentX, -extentY);
	_vertices[1] = glm::vec2(-extentX, extentY);
	_vertices[2] = glm::vec2(extentX, extentY);
	_vertices[3] = glm::vec2(extentX, -extentY);

	_uvs[0] = glm::vec2(0, 0);
	_uvs[1] = glm::vec2(0, _size.y);
	_uvs[2] = glm::vec2(_size.x, _size.y);
	_uvs[3] = glm::vec2(_size.x, 0);

	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 2;
	
	_indices[3] = 2;
	_indices[4] = 3;
	_indices[5] = 0;
}
