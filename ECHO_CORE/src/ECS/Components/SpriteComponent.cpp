#include "Core/ECS/Components/SpriteComponent.h"

#include "Core/Resources/AssetManager.h"

#include <Logger/Logger.h>

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    void SpriteComponent::CreateLuaBind(sol::state &lua,
        ECHO_CORE::ECS::Registry &registry)
    {
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
                        .Colour = ECHO_RENDERING::Color{255, 255, 255, 255},
                        .Start_X = start_x,
                        .Start_Y = start_y,
                        .Layer = layer,
                        .Texture_Name = tex_name
                    };
                }
            ),
            "Texture_Name", &SpriteComponent::Texture_Name,
            "Width", &SpriteComponent::Width,
            "Height", &SpriteComponent::Height,
            "Start_X", &SpriteComponent::Start_X,
            "Start_Y", &SpriteComponent::Start_Y,
            "Layer", &SpriteComponent::Layer,
            "generateUVs",
            [&](SpriteComponent &sprite)
            {
                const auto &asset_manager = registry.GetContext<
                    std::shared_ptr<ECHO_RESOURCES::AssetManager>>();
                auto &texture = asset_manager->GetTexture(sprite.Texture_Name);

                if (texture.GetID() == 0)
                {
                    ECHO_ERROR("Failed to generate uvs - Texture [{}] does "
                        "not exist", sprite.Texture_Name);
                    return;
                }

                sprite.GenerateUVs(texture.GetWidth(), texture.GetHeight());
            }
        );
    }
}
