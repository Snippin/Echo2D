#include "Core/ECS/Components/CircleColliderComponent.h"

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    void CircleColliderComponent::CreateLuaBind(sol::state &lua)
    {
        lua.new_usertype<CircleColliderComponent>(
            "CircleCollider",
            "type_id", entt::type_hash<CircleColliderComponent>::value,
            sol::call_constructor,
            sol::factories(
                [](float radius)
                {
                    return CircleColliderComponent{
                        .Radius = radius
                    };
                },
                [](float radius, glm::vec2 offset)
                {
                    return CircleColliderComponent{
                        .Radius = radius,
                        .Offset = offset
                    };
                }
            ),
            "Radius", &CircleColliderComponent::Radius,
            "Offset", &CircleColliderComponent::Offset,
            "IsColliding", &CircleColliderComponent::IsColliding
        );
    }
}
