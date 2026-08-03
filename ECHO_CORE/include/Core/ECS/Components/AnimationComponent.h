#pragma once

#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    struct AnimationComponent
    {
        int Frames{1};
        int FrameRate{1};
        int FrameOffset{1};
        int CurrentFrame{1};
        bool IsVerical{};

        static void CreateLuaBind(sol::state &lua);
    };
}
