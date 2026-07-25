#pragma once

#include "../ECS/Registry.h"

#include <sol/sol.hpp>

namespace ECHO_CORE::SYSTEMS
{
    class ScriptSystem
    {
    public:
        ScriptSystem(ECHO_CORE::ECS::Registry &registry);
        ~ScriptSystem() = default;

        bool LoadMainScript(sol::state &lua);
        void Update();
        void Render();

    private:
        ECHO_CORE::ECS::Registry &registry;
        bool main_loaded;
    };
}
