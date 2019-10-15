#include "Core.h"
#include "CharacterController.h"
#include "Rendering/Sprite.h"
#include "Physics/Ray.h"
#include "Physics/Intersection.h"

void Platformer::CharacterController::move(glm::vec2 moveDirection)
{
	const float offsetX = _moveSpeed * moveDirection.x;
	_velocity.x = offsetX;
	if (offsetX != 0)
	{
		_sprite->flipX(offsetX < 0);
	}
}

void Platformer::CharacterController::jump()
{
	if (isGrounded())
	{
		_velocity.y = _jumpVelocity;
	}
}

void Platformer::CharacterController::jump(glm::vec2 from, glm::vec2 to)
{
	if (!isGrounded())
	{
		return;
	}

	const glm::vec2 target = to - from;

	float theta = glm::pow(_jumpVelocity, 4) - _gravity * (_gravity * target.x * target.x + 2.f * target.y * _jumpVelocity * _jumpVelocity);
	theta =  _jumpVelocity * _jumpVelocity + glm::sqrt(theta);
	theta = theta / (_gravity * target.x);
	theta = glm::atan(theta);

	_velocity.x = glm::sign(target.x) * _jumpVelocity * glm::cos(theta);
	_velocity.y = glm::abs(_jumpVelocity * glm::sin(theta));
}

void Platformer::CharacterController::jumpDebugWindow() const
{
	static bool isOpen = true;
	if (ImGui::Begin("Jump debug", &isOpen))
	{
		static glm::vec2 from, to;
		ImGui::InputFloat2("From", reinterpret_cast<float*>(&from), "%.1f", ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat2("To", reinterpret_cast<float*>(&to), "%.1f", ImGuiInputTextFlags_CharsDecimal);

		const glm::vec2 target = to - from;

		float theta = glm::pow(_jumpVelocity, 4) - _gravity * (_gravity * target.x * target.x + 2.f * target.y * _jumpVelocity * _jumpVelocity);
		theta = _jumpVelocity * _jumpVelocity + glm::sqrt(theta);
		theta = theta / (_gravity * target.x);
		theta = glm::atan(theta);

		glm::vec2 velocity;
		velocity.x = _jumpVelocity * glm::cos(theta);
		velocity.y = glm::abs(_jumpVelocity * glm::sin(theta));

		ImGui::Separator();
		ImGui::LabelText("Result velocity:", "(%.1f, %.1f)", velocity.x, velocity.y);
		ImGui::LabelText("Current velocity:", "(%.1f, %.1f)", _velocity.x, _velocity.y);
	}

	ImGui::End();
}

void Platformer::CharacterController::update(float deltaTime)
{
	_velocity.y -= _gravity * deltaTime;
	
	glm::vec2 frameVelocity = _velocity * deltaTime;
	moveOneFrame(frameVelocity);

	_sprite->move(frameVelocity);
}

glm::vec2 Platformer::CharacterController::getCurrentVelocity() const
{
	return _velocity;
}

void Platformer::CharacterController::moveOneFrame(glm::vec2& velocity)
{
	Engine::Intersection i {};
	if (velocity.y < 0)
	{
		if (bottomCollisions(velocity, i))
		{
			velocity.y = -(i.distance - _skinWidth);
			_isGrounded = true;
			_velocity.y = 0;
		}
		else
		{
			_isGrounded = false;
		}
	}
	else if (velocity.y > 0)
	{
		if (topCollisions(velocity, i))
		{
			velocity.y = i.distance - _skinWidth;
			_velocity.y = 0.f;
		}

		_isGrounded = false;
	}

	if (velocity.x > 0 && rightCollisions(velocity, i))
	{
		velocity.x = i.distance - _skinWidth;
	}
	else if (velocity.x < 0 && leftCollisions(velocity, i))
	{
		velocity.x = -(i.distance - _skinWidth);
	}
}

bool Platformer::CharacterController::bottomCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.min + glm::vec2(0, _skinWidth);
	Engine::Ray r(origin, { 0, -1 });

	const float maxDistance = glm::abs(velocity.y) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _verticalRays + 1; ++i)
	{
		r.setOrigin(origin + glm::vec2(i * _verticalRaySpacing, 0));
		
		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
}

bool Platformer::CharacterController::topCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.max + glm::vec2(0, -_skinWidth);
	Engine::Ray r(origin, { 0, 1 });

	const float maxDistance = glm::abs(velocity.y) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _verticalRays + 1; ++i)
	{
		r.setOrigin(origin - glm::vec2(i * _verticalRaySpacing, 0));
		
		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
}

bool Platformer::CharacterController::rightCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.max + glm::vec2(-_skinWidth, 0);
	Engine::Ray r(origin, { 1, 0 });

	const float maxDistance = glm::abs(velocity.x) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _horizontalRays + 1; ++i)
	{
		r.setOrigin(origin - glm::vec2(0, i * _horizontalRaySpacing));

		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
}

bool Platformer::CharacterController::leftCollisions(glm::vec2 velocity, Engine::Intersection& it) const
{
	Engine::Intersection intersec{  };
	glm::vec2 origin = _bounds.min + glm::vec2(_skinWidth, 0);
	Engine::Ray r(origin, { -1, 0 });

	const float maxDistance = glm::abs(velocity.x) + _skinWidth;
	float distance = maxDistance;
	for (int i = 0; i < _horizontalRays + 1; ++i)
	{
		r.setOrigin(origin + glm::vec2(0, i * _horizontalRaySpacing));
		bool isHit = _raycaster.raycast(r, distance, Engine::BoundingBox::PLATFORM, intersec);
		if (isHit && intersec.distance < distance)
		{
			distance = intersec.distance;
		}
	}

	it.distance = distance;
	return distance < maxDistance;
}



bool Platformer::CharacterController::isGrounded() const
{
	return _isGrounded;
}

void Platformer::CharacterController::setValues(float moveSpeed, float jumpHeight, float topJumpTime)
{
	_moveSpeed = moveSpeed;
	_jumpHeight = jumpHeight;
	_topJumpTime = topJumpTime;

	_gravity = 2.f * _jumpHeight / (_topJumpTime * _topJumpTime);
	_jumpVelocity = _gravity * _topJumpTime;
}



Platformer::CharacterController::CharacterController(const Engine::Raycaster& raycaster, Engine::Sprite* sprite) :
	_bounds(sprite->getBoundingBox()),
	_raycaster(raycaster),
	_sprite(sprite)
{
}


