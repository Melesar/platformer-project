#pragma once

#include <Core.h>
#include <bitset>
#include <cassert>
#include "MPL/MPL.hpp"

#include "Entity.h"

namespace ECS
{
    using UInt = std::size_t;

    template <typename... TComponents>
    struct ENGINE_API Settings
    {
        using Components =  MPL::TypeList<TComponents...>;

        static constexpr UInt componentsCount() { return Components::size; }

        template<typename T>
        static constexpr UInt componentId() { return MPL::IndexOf<T, Components>{};}

        template<typename T>
        static constexpr bool isComponent() { return MPL::Contains<T, Components>{}; }

        template<typename T>
        static constexpr UInt componentSize() { return sizeof(T);}

        using Bitset = std::bitset<componentsCount()>;
    };

    template <typename TSettings>
    class ENGINE_API Manager
    {
    private:
        using Settings = TSettings;
        using Components = typename Settings::Components;
        using Bitset = typename Settings::Bitset;
        using Entity = ECS::Entity<Settings>;
        using EntityHandle = const Entity&;

        template <typename... Ts>
        using TupleOfVectors = std::tuple<std::vector<Ts>...>;

        using Storage = MPL::Rename<TupleOfVectors, Components>;

    public:

        EntityHandle createEntity ()
        {
            growIfNeeded();

            Entity& e = _entities[_nextEntityIndex];
            e.index = _nextEntityIndex++;
            e.isAlive = true;

            _isDirty = true;

            return e;
        }

        void deleteEntity(EntityHandle e)
        {
            _isDirty = true;
            _entities[e.index].isAlive = false;
        }

        template<typename T, typename... TArgs>
        T& addComponent(EntityHandle e, TArgs&&... args)
        {
            assertIsComponent<T>();

            componentBit<T>(e.index) = true;

            T& c (getComponent<T>(e));
            c = T(std::forward<decltype(args)>(args)...);  //use TArgs instead of decltype(args)?
            return c;
        }

        template<typename T>
        bool hasComponent(EntityHandle e)
        {
            assertIsComponent<T>();
            return componentBit<T>(e.index);
        }

        template<typename T>
        T& getComponent(EntityHandle e)
        {
            assertIsComponent<T>();
            assert(hasComponent<T>(e));

            return std::get<std::vector<T>>(_componentsStorage)[e.index];
        }

        template<typename T>
        void removeComponent(EntityHandle e)
        {
            assertIsComponent<T>();

            componentBit<T>(e.index) = false;
        }

        bool isAlive(EntityHandle e)
        {
            return isAlive(e.index);
        }

        void clear()
        {
            _nextEntityIndex = 0;
        }

        void refresh()
        {
            if (!_isDirty)
            {
                return;
            }

            if (_nextEntityIndex == 0)
            {
                _currentEntityIndex = 0;
                _isDirty = false;
                return;
            }

            _currentEntityIndex = _nextEntityIndex = refreshImplementation();
            _isDirty = false;
        }

        template <typename TFunc>
        void forEntities(TFunc&& func)
        {
            for (int i = 0; i < _currentEntityIndex; ++i)
            {
                func(_entities[i]);
            }
        }


        Manager()
        {
            grow(100);
        }

    private:

        void grow(UInt newCapacity)
        {
            assert(newCapacity > _currentCapacity);

            _entities.resize(newCapacity);

            for (UInt i = _currentCapacity; i < newCapacity; ++i)
            {
                Entity& e = _entities[i];
                e.index = i;
                e.bitset.reset();
                e.isAlive = false;
            }

            MPL::forTuple([this, newCapacity] (auto& vec)
            {
               vec.resize(newCapacity);
            }, _componentsStorage);

            _currentCapacity = newCapacity;
        }

        void growIfNeeded()
        {
            if (_nextEntityIndex >= _currentCapacity)
            {
                grow(_currentCapacity * 2);
            }
        }

        template<typename T>
        auto componentBit(UInt entityIndex)
        {
            static_assert(Settings::template isComponent<T>());

            return _entities[entityIndex].bitset[Settings::template componentId<T>()];
        }

        template<typename T>
        static constexpr void assertIsComponent()
        {
            static_assert(Settings::template isComponent<T>());
        }

        bool isAlive(UInt entityIndex)
        {
            return _entities[entityIndex].isAlive;
        }

        UInt refreshImplementation()
        {
            UInt aliveIndex {_nextEntityIndex - 1};
            UInt deadIndex {0};

            while(true)
            {
                for(; true; ++deadIndex)
                {
                    if (deadIndex > aliveIndex) return deadIndex;
                    if (!isAlive(deadIndex)) break;
                }

                for(; true; --aliveIndex)
                {
                    if (aliveIndex <= deadIndex) return deadIndex;
                    if (isAlive(aliveIndex)) break;
                }

                assert(!isAlive(deadIndex));
                assert(isAlive(aliveIndex));

                std::swap(_entities[deadIndex], _entities[aliveIndex]);
                MPL::forTuple([deadIndex, aliveIndex] (auto& vec) {
                    std::swap(vec[deadIndex], vec[aliveIndex]);
                }, _componentsStorage);

                ++deadIndex;
                --aliveIndex;
            }
        }

    private:

        bool _isDirty {false};

        std::vector<Entity> _entities;
        Storage _componentsStorage;

        UInt _currentCapacity {0};
        UInt _currentEntityIndex{0};
        UInt _nextEntityIndex{0};
    };
}