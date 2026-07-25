#pragma once

#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    struct ScriptComponent
    {
        sol::protected_function Update{sol::lua_nil};
        sol::protected_function Render{sol::lua_nil};
    };
}
