#pragma once

#include <Core.h>
#include "System.h"

namespace ECS
{
    template<typename TSettings, typename... TSystems>
    class ENGINE_API SystemsGroup
    {
        using SystemsList = MPL::TypeList<TSystems...>;

    public:

        void update()
        {
            MPL::forTuple([this](auto& systemPtr) {
                systemPtr->update();
                _manager.refresh();
            }, _systems);
        }

        template<typename T>
        T& getSystem()
        {
            static_assert(MPL::Contains<T, SystemsList >{});

            return std::get<T>(_systems);
        }

        template<typename T>
        const T& getSystem() const
        {
            static_assert(MPL::Contains<T, SystemsList >{});

            return std::get<T>(_systems);
        }

        explicit SystemsGroup(Manager<TSettings>& manager) : _manager(manager)
        {
            MPL::forTypes<SystemsList>([this, &manager](auto t){
               using SystemType = typename decltype(t)::type;
               std::get<std::unique_ptr<SystemType>>(_systems) = std::make_unique<SystemType>(manager);
            });
        }

    private:
        std::tuple<std::unique_ptr<TSystems>...> _systems;
        Manager<TSettings>& _manager;
    };



    template<typename TSettings, typename... TGroups>
    class ENGINE_API SystemsQueue
    {
        using GroupsList = MPL::TypeList<TGroups...>;
    public:

        void update()
        {
            MPL::forTuple([](auto& groupPtr) {
                groupPtr->update();
            }, _groups);
        }

        explicit SystemsQueue (Manager<TSettings>& manager)
        {
            MPL::forTypes<GroupsList>([this, &manager](auto t){
                using GroupType = typename decltype(t)::type;
                std::get<std::unique_ptr<GroupType>>(_groups) = std::make_unique<GroupType>(manager);
            });
        }

    private:

        std::tuple<std::unique_ptr<TGroups>...> _groups;
    };
}