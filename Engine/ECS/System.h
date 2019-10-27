#pragma once

#include <Core.h>
#include "Manager.h"

namespace ECS
{
    template <typename TSettings, typename TSystem, typename... TComponents>
    class ENGINE_API ComponentSystem
    {
    protected:

        using EntityHandle = const Entity<TSettings>&;

    private:

        using Components = MPL::TypeList<TComponents...>;
        using Bitset = typename TSettings::Bitset;

    public:

        void update()
        {
            _manager.forEntities([this](EntityHandle entity){
               if (entityMatch(entity))
               {
                   TSystem::onUpdate(entity, _manager, _manager.template getComponent<TComponents>(entity)...);
               }
            });
        }

        explicit ComponentSystem(Manager<TSettings>& manager) : _manager(manager)
        {
            initSignature();
        }

    private:

        bool entityMatch(EntityHandle e)
        {
            return e.isAlive && (e.bitset & _systemSignature) == _systemSignature;
        }


        void initSignature()
        {
            MPL::forTypes<Components>([this](auto t)
            {
                _systemSignature[TSettings::template componentId<typename decltype(t)::type>()] = true;
            });
        }

    private:

        Manager<TSettings>& _manager;
        Bitset _systemSignature;
    };
}


