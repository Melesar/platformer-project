#include <ECS/Manager.h>
#include "Application.h"

#undef main

struct PositionComponent
{
    float x, y;
};

struct VelocityComponent
{
    glm::vec2 velocity;
};

using Settings = ECS::Settings<PositionComponent, VelocityComponent>;

int main(int argc, char **argv)
{
    ECS::Manager<Settings> manager;
    auto entity1 = manager.createEntity();
    auto &position = manager.addComponent<PositionComponent>(entity1);
    position.x = 10.f;
    position.y = 11.f;

    assert(manager.hasComponent<PositionComponent>(entity1));

    auto entity2 = manager.createEntity();
    auto &velocity = manager.addComponent<VelocityComponent>(entity2);
    velocity.velocity = {1, 0};

    assert(manager.hasComponent<VelocityComponent>(entity2));

    manager.refresh();
//	Platformer::Application app;
//	app.run(argc, argv);

    return 0;
}