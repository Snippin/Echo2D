#include "Core/Systems/RenderSystem.h"

#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/Resources/AssetManager.h"

#include <Logger/Logger.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Shader.h>

namespace ECHO_CORE::SYSTEMS
{
    RenderSystem::RenderSystem(ECHO_CORE::ECS::Registry &registry) :
        registry{registry}, batch_renderer{nullptr}
    {
        batch_renderer = std::make_unique<ECHO_RENDERING::BatchRenderer>();
    }

    void RenderSystem::Render()
    {
        auto &camera = registry.GetContext<std::shared_ptr<
            ECHO_RENDERING::Camera2D>>();
        auto &asset_manager = registry.GetContext<std::shared_ptr<
            ECHO_RESOURCES::AssetManager>>();

        auto &sprite_shader = asset_manager->GetShader("basic");
        auto cam_mat = camera->GetCameraMatrix();

        if (sprite_shader.ShaderProgramID() == 0)
        {
            ECHO_ERROR("Sprite shader program has not been created properly");
            return;
        }

        sprite_shader.Enable();
        sprite_shader.SetUniformMat4("uProjection", cam_mat);

        batch_renderer->Begin();

        auto view = registry.Get().view<ECHO_CORE::ECS::TransformComponent,
            ECHO_CORE::ECS::SpriteComponent>();

        for (const auto &entity : view)
        {
            const auto &transform =
                view.get<ECHO_CORE::ECS::TransformComponent>(entity);
            const auto &sprite =
                view.get<ECHO_CORE::ECS::SpriteComponent>(entity);

            if (sprite.Texture_Name.empty() || sprite.IsHidden)
            {
                continue;
            }

            const auto &texture =
                asset_manager->GetTexture(sprite.Texture_Name);
            if (texture.GetID() == 0)
            {
                ECHO_ERROR("Texture [{}] does not exist", sprite.Texture_Name);
                return;
            }

            glm::vec4 sprite_rect{
                transform.Position.x - sprite.Width * 0.5f,
                transform.Position.y - sprite.Height * 0.5f,
                sprite.Width, sprite.Height};
            glm::vec4 uv_rect{sprite.Uvs.X, sprite.Uvs.Y, sprite.Uvs.UV_Width,
                sprite.Uvs.UV_Height};

            glm::mat4 model{1.f};

            if (transform.Rotation != 0.f || sprite.Rotation != 0 ||
                transform.Scale.x != 1.f || transform.Scale.y != 1.f)
            {
                // Move to transform current position
                model =
                    glm::translate(model, glm::vec3{transform.Position, 0.f});

                if (transform.Rotation != 0.f || sprite.Rotation != 0)
                {
                    model = glm::rotate(model,
                        glm::radians(transform.Rotation + sprite.Rotation),
                        glm::vec3{0.f, 0.f, 1.f});
                }

                if (transform.Scale.x != 1.f || transform.Scale.y != 1.f)
                {
                    model = glm::scale(model, glm::vec3(transform.Scale, 1.f));
                }

                // Move to transform to original position
                model =
                    glm::translate(model, glm::vec3{-transform.Position, 0.f});
            }

            batch_renderer->AddSprite(sprite_rect, uv_rect, texture.GetID(),
                sprite.Layer, model, sprite.Colour);
        }

        batch_renderer->End();
        batch_renderer->Render();

        sprite_shader.Disable();
    }
}
