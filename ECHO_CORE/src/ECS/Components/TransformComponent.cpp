#include "Core/ECS/Components/TransformComponent.h"

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    void TransformComponent::CreateLuaBind(sol::state &lua)
    {
        lua.new_usertype<TransformComponent>(
            "Transform",
            "type_id", &entt::type_hash<TransformComponent>::value,
            sol::call_constructor,
            sol::factories(
                [](glm::vec2 position, glm::vec2 scale, float rotation)
                {
                    return TransformComponent{
                        .Position = position,
                        .Scale = scale,
                        .Rotation = rotation
                    };
                },
                [](float pos_x, float pos_y, float scale_x, float scale_y,
                    float rotation)
                {
                    return TransformComponent{
                        .Position = glm::vec2{pos_x, pos_y},
                        .Scale = glm::vec2{scale_x, scale_y},
                        .Rotation = rotation
                    };
                }
            ),
            "Position",
            [](TransformComponent &transform)
            {
                return std::make_tuple(transform.Position.x, transform.Position.y);
            },
            "Scale",
            [](TransformComponent &transform)
            {
                return std::make_tuple(transform.Scale.x, transform.Scale.y);
            },
            "Rotation", &TransformComponent::Rotation,
            "setPos",
            [](TransformComponent &transform, float x, float y)
            {
                transform.Position = glm::vec2{x, y};
            },
            "setScale",
            [](TransformComponent &transform, float x, float y)
            {
                transform.Scale = glm::vec2{x, y};
            }
        );
    }
}
