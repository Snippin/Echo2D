#include "Core/ECS/Components/SpriteComponent.h"

#include "Core/Resources/AssetManager.h"

#include <Logger/Logger.h>

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    void SpriteComponent::CreateLuaBind(sol::state &lua,
        ECHO_CORE::ECS::Registry &registry)
    {
        lua.new_usertype<ECHO_RENDERING::Color>(
            "Color",
            "type_id", &entt::type_hash<ECHO_RENDERING::Color>::value,
            sol::call_constructor,
            sol::factories(
                []()
                {
                    return ECHO_RENDERING::Color{};
                },
                [](GLubyte r, GLubyte g, GLubyte b, GLubyte a)
                {
                    return ECHO_RENDERING::Color{.R = r, .G = g,.B = b, .A = a};
                }
            ),
            "R", &ECHO_RENDERING::Color::R,
            "G", &ECHO_RENDERING::Color::G,
            "B", &ECHO_RENDERING::Color::B,
            "A", &ECHO_RENDERING::Color::A
        );

        lua.new_usertype<UVs>(
            "UVs",
            "type_id", &entt::type_hash<UVs>::value,
            sol::call_constructor,
            sol::factories(
                []() { return UVs{}; },
                [](float x, float y) { return UVs{.X = x, .Y = y}; }
            ),
            "X", &UVs::X,
            "Y", &UVs::Y,
            "UVWidth", &UVs::UVWidth,
            "UVHeight", &UVs::UVHeight
        );

        lua.new_usertype<SpriteComponent>(
            "Sprite",
            "type_id", &entt::type_hash<SpriteComponent>::value,
            sol::call_constructor,
            sol::factories(
                [](const std::string &tex_name, float width, float height,
                    int start_x, int start_y, int layer)
                {
                    return SpriteComponent{
                        .Width = width,
                        .Height = height,
                        .Uvs = UVs{},
                        .Color = ECHO_RENDERING::Color{255, 255, 255, 255},
                        .StartX = start_x,
                        .StartY = start_y,
                        .Layer = layer,
                        .Rotation = 0,
                        .IsHidden = false,
                        .TextureName = tex_name
                    };
                },
                [](const std::string &tex_name, float width, float height,
                    int start_x, int start_y, int layer, float rotation)
                {
                    return SpriteComponent{
                        .Width = width,
                        .Height = height,
                        .Uvs = UVs{},
                        .Color = ECHO_RENDERING::Color{255, 255, 255, 255},
                        .StartX = start_x,
                        .StartY = start_y,
                        .Layer = layer,
                        .Rotation = rotation,
                        .IsHidden = false,
                        .TextureName = tex_name
                    };
                }
            ),
            "TextureName", &SpriteComponent::TextureName,
            "Width", &SpriteComponent::Width,
            "Height", &SpriteComponent::Height,
            "UVs", &SpriteComponent::Uvs,
            "Color", &SpriteComponent::Color,
            "StartX", &SpriteComponent::StartX,
            "StartY", &SpriteComponent::StartY,
            "Layer", &SpriteComponent::Layer,
            "IsHidden", &SpriteComponent::IsHidden,
            "Rotation", &SpriteComponent::Rotation,
            "GenerateUVs",
            [&](SpriteComponent &sprite)
            {
                const auto &asset_manager = registry.GetContext<
                    std::shared_ptr<ECHO_RESOURCES::AssetManager>>();
                auto &texture = asset_manager->GetTexture(sprite.TextureName);

                if (texture.GetID() == 0)
                {
                    ECHO_ERROR("Failed to generate uvs - Texture [{}] does "
                        "not exist", sprite.TextureName);
                    return;
                }

                sprite.GenerateUVs(texture.GetWidth(), texture.GetHeight());
            }
        );
    }
}
