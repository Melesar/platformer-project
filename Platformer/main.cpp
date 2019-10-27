#include "Application.h"
#include <ECS/ECS.h>

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


class MovementSystem : public ECS::ComponentSystem<Settings, MovementSystem, PositionComponent, VelocityComponent>
{

public:

    static void onUpdate(EntityHandle e, ECS::Manager<Settings>& mgr, PositionComponent& position, VelocityComponent& velocity)
    {
        std::cout << "Position x: " << position.x << std::endl;
        if (position.x > 2)
        {
            mgr.deleteEntity(e);
        }

    }


    explicit MovementSystem(ECS::Manager<Settings>& manager) : ECS::ComponentSystem<Settings, MovementSystem, PositionComponent, VelocityComponent>(manager)
    {
    }

};

class RenderingSystemGroup : public ECS::SystemsGroup<Settings, MovementSystem>
{
public:
    explicit RenderingSystemGroup(ECS::Manager<Settings> &manager) : SystemsGroup(manager)
    {}
};

class SystemsQueue : public ECS::SystemsQueue<Settings, RenderingSystemGroup>
{
public:
    explicit SystemsQueue(ECS::Manager<Settings> &manager) : ECS::SystemsQueue<Settings, RenderingSystemGroup>(manager)
    {}
};

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

    SystemsQueue systemsQueue (manager);

    std::cout << "Update #1" << std::endl;
    systemsQueue.update();
    std::cout << "Update #2" << std::endl;
    systemsQueue.update();
    std::cout << "Update #3" << std::endl;
    systemsQueue.update();


//    assert(manager.getComponent<PositionComponent>(entity3).x == 3.f);
//    assert(manager.getComponent<PositionComponent>(entity2).x == 2.f);
//    assert(!manager.isAlive(entity2));
//    assert(manager.isAlive(entity3));

//	Platformer::Application app;
//	app.run(argc, argv);

    return 0;
}