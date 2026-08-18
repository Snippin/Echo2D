#include "Core/Systems/AnimationSystem.h"

#include "Core/ECS/Components/AnimationComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TransformComponent.h"

#include <SDL.h>

namespace ECHO_CORE::SYSTEMS
{
    AnimationSystem::AnimationSystem(ECHO_CORE::ECS::Registry &registry) :
        registry{registry}

    {
    }

    void AnimationSystem::Update()
    {
        auto view = registry.Get().view<
            ECHO_CORE::ECS::AnimationComponent,
            ECHO_CORE::ECS::SpriteComponent,
            ECHO_CORE::ECS::TransformComponent>();

        for (auto entity : view)
        {
            const auto &transform =
                view.get<ECHO_CORE::ECS::TransformComponent>(entity);
            auto &sprite =
                view.get<ECHO_CORE::ECS::SpriteComponent>(entity);
            auto &animation =
                view.get<ECHO_CORE::ECS::AnimationComponent>(entity);

            if (animation.Frames <= 0)
            {
                continue;
            }

            // Check if looped and current frame = max frames
            if (!animation.IsLooped &&
                animation.CurrentFrame >= animation.Frames - 1)
            {
                continue;
            }

            // Get current frame
            animation.CurrentFrame = ((SDL_GetTicks() - animation.StartTime) *
                animation.FrameRate / 1000) % animation.Frames;

            if (animation.IsVerical)
            {
                sprite.Uvs.Y = animation.CurrentFrame * sprite.Uvs.UV_Height;
                sprite.Uvs.X = animation.FrameOffset * sprite.Uvs.UV_Width;
            }
            else
            {
                sprite.Uvs.X = (animation.CurrentFrame * sprite.Uvs.UV_Width) +
                    (animation.FrameOffset * sprite.Uvs.UV_Width);
            }
        }
    }
}
