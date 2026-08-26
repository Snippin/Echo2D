#pragma once
#pragma once

#include <sol/sol.hpp>

namespace ECHO_CORE
{
    namespace ECS
    {
        class Registry;
    }

    namespace SCRIPTING
    {
        struct RenderingBindings
        {
            static void CreateLuaBind(sol::state &lua,
                ECS::Registry &registry);
        };
    }
}
