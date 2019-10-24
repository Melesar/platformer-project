#pragma once

#include <Core.h>


namespace MPL
{
    // Compile-time list of types.
    template<typename... Ts>
    struct TypeList
    {
        // Size of the list.
        static constexpr std::size_t size{sizeof...(Ts)};
    };
}
