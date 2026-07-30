#pragma once

#include <sol/sol.hpp>

namespace ECHO_CORE::SCRIPTING
{
    struct GLMBindings
    {
        static void CreateLuaBind(sol::state &lua);
    };

}