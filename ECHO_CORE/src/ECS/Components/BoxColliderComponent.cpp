#include "Core/ECS/Components/BoxColliderComponent.h"

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    void BoxColliderComponent::CreateLuaBind(sol::state &lua)
    {
        lua.new_usertype<BoxColliderComponent>(
            "BoxCollider",
            "type_id", entt::type_hash<BoxColliderComponent>::value,
            sol::call_constructor,
            sol::factories(
                [](int width, int height)
                {
                    return BoxColliderComponent{
                        .Width = width,
                        .Height = height
                    };
                },
                [](int width, int height, glm::vec2 offset)
                {
                    return BoxColliderComponent{
                        .Width = width,
                        .Height = height,
                        .Offset = offset
                    };
                }
            ),
            "Width", &BoxColliderComponent::Width,
            "Height", &BoxColliderComponent::Height,
            "Offset", &BoxColliderComponent::Offset,
            "IsColliding", &BoxColliderComponent::IsColliding
        );
    }
}
