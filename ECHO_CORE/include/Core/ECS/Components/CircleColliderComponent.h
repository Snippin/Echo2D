#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    struct CircleColliderComponent
    {
        float Radius{0};
        glm::vec2 Offset{glm::vec2{0}};
        bool IsColliding{};

        static void CreateLuaBind(sol::state &lua);
    };
}
