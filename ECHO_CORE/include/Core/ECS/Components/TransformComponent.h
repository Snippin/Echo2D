#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    struct TransformComponent
    {
        glm::vec2 Position{glm::vec2{1.f}};
        glm::vec2 Scale{glm::vec2{1.f}};
        float Rotation{0.f};

        static void CreateLuaBind(sol::state &lua);
    };
}
