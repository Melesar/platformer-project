#pragma once

#include <type_traits>
#include "Type.hpp"
#include "TypeList.hpp"
#include "Rename.hpp"
#include "Concat.hpp"
#include "Map.hpp"
#include "TypeListOps.hpp"
#include "Repeat.hpp"
#include "All.hpp"
#include "IndexOf.hpp"
#include "Count.hpp"
#include "ContainsAll.hpp"
#include "Filter.hpp"
#include "Unique.hpp"



namespace MPL
{

//    template<typename TTypeList>
//    constexpr auto size() noexcept
//    {
//        return size<TTypeList>();
//    }
//
//    template<typename TTypeList, typename TFunction>
//    constexpr void forTypes(TFunction &&mFunction) noexcept
//    {
//        forTypes<TTypeList>(ECS_FWD(mFunction));
//    }
//
//    template<typename... Ts, typename TFunction>
//    constexpr void forTypesExp(TFunction &&mFunction) noexcept
//    {
//        forTypes<TypeList < Ts...>>
//        (ECS_FWD(mFunction));
//    }
}
