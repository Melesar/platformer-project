#pragma once
#include <Core.h>

namespace ECS
{
    template <typename TSettings>
    struct ENGINE_API Entity
    {
        using Settings = TSettings;
        using Bitset = typename Settings::Bitset;

        Bitset bitset;

        std::size_t index {0};

        bool isAlive {false};
    };
}