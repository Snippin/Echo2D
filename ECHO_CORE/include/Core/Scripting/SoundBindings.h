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
        struct SoundBindings
        {
            static void CreateLuaBind(sol::state &lua,
                ECS::Registry &registry);
        };
    }
}
