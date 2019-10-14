#include "Core.h"
#include "Enemy.h"
#include "Rendering/Sprite.h"
#include "Physics/Raycaster.h"
#include "Player/Player.h"
#include "AI/AIMovementConstants.h"


Platformer::Enemy::Enemy(Engine::Sprite* sprite, const Player& player, const Engine::Raycaster& raycaster, const Navmesh& navmesh) :
	_sprite(sprite),
	_player(player),
	_controller(raycaster, sprite),
	_navmesh(navmesh)
{
	sprite->setLayer(Engine::BoundingBox::ENEMY);
	sprite->setSortingOrder(20);

	_controller.setValues(MOVEMENT_SPEED, JUMP_HEIGHT, JUMP_TIME);
}

void showPath(Platformer::NavmeshPath path)
{
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	if (corner != -1)
	{
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Navmesh path", nullptr, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Current path length: %d", path.size());
		ImGui::Separator();
		int index = 0;
		while (!path.empty())
		{
			Platformer::NavmeshLink link = path.front();
			ImVec4 color;
			switch (link.type)
			{
			case Platformer::WALKABLE:
				color = { 1, 1, 1, 1 };
				break;
			case Platformer::FALL:
				color = { 0, 1, 0, 1 };
				break;
			case Platformer::JUMP:
				color = { 0, 0, 1, 1 };
				break;
			}
			ImGui::TextColored(color, "%d. (%.1f, %.1f) -> (%.1f, %.1f)", ++index, link.from->position.x, link.from->position.y, link.to->position.x, link.to->position.y);
			
			path.pop_front();
		}
	}
	ImGui::End();
}

void Platformer::Enemy::update(float deltaTime)
{
	_navmesh.getPath(_sprite->getPosition(), _player.getPosition(), _currentPath);

	if (_currentPath.empty())
	{
		_controller.update(deltaTime);
		return;
	}

	_currentTarget = &_currentPath.front();
	switch (_currentTarget->type)
	{
	case WALKABLE:
	case FALL:
		moveToPoint(_currentTarget->to->position);
		break;
	case JUMP:
		_controller.jump(_currentTarget->from->position, _currentTarget->to->position);
		break;
	}

	_controller.update(deltaTime);
}

void Platformer::Enemy::moveToPoint(glm::vec2 destination)
{
	_controller.move({ glm::sign(destination.x - _sprite->getPosition().x), 0 });
}

void Platformer::Enemy::moveInDirection(short direction)
{
	_controller.move({ direction, 0.f });
}
