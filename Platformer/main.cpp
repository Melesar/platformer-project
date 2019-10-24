#include <ECS/Manager.h>
#include "Application.h"

#undef main

struct PositionComponent
{
    float x, y;

    PositionComponent() = default;
    PositionComponent(float x, float y) : x(x), y(y){}
};

struct VelocityComponent
{
    glm::vec2 velocity;

    VelocityComponent() = default;
    explicit VelocityComponent(glm::vec2 velocity) : velocity(velocity) {}
};

using Settings = ECS::Settings<PositionComponent, VelocityComponent>;

int main(int argc, char **argv)
{
    ECS::Manager<Settings> manager;
    auto& entity1 = manager.createEntity();
    manager.addComponent<PositionComponent>(entity1, 1.f, 11.f);
    manager.addComponent<VelocityComponent>(entity1, glm::vec2(1.f, 1.2f));

    auto& entity2 = manager.createEntity();
    manager.addComponent<PositionComponent>(entity2, 2.f, 14.f);
    manager.addComponent<VelocityComponent>(entity2, glm::vec2(2.f, 9.3f));

    auto& entity3 = manager.createEntity();
    manager.addComponent<PositionComponent>(entity3, 3.f, 14.f);
    manager.addComponent<VelocityComponent>(entity3, glm::vec2(3.f, 9.3f));

    manager.refresh();

    manager.deleteEntity(entity2);

    manager.refresh();

    assert(manager.getComponent<PositionComponent>(entity3).x == 3.f);
    assert(manager.getComponent<PositionComponent>(entity2).x == 2.f);
    assert(!manager.isAlive(entity2));
    assert(manager.isAlive(entity3));

//	Platformer::Application app;
//	app.run(argc, argv);

    return 0;
}