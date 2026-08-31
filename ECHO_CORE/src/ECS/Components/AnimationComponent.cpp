#include "Core/ECS/Components/AnimationComponent.h"

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    void AnimationComponent::CreateLuaBind(sol::state &lua)
    {
        lua.new_usertype<AnimationComponent>(
            "Animation",
            "type_id", &entt::type_hash<AnimationComponent>::value,
            sol::call_constructor,
            sol::factories(
                [](int frames, int frame_rate, int frame_offset, bool vertical,
                    bool looped)
                {
                    return AnimationComponent{
                        .Frames = frames,
                        .FrameRate = frame_rate,
                        .FrameOffset = frame_offset,
                        .IsVerical = vertical,
                        .IsLooped = looped,
                    };
                }
            ),
            "Frames", &AnimationComponent::Frames,
            "FrameRate", &AnimationComponent::FrameRate,
            "FrameOffset", &AnimationComponent::FrameOffset,
            "CurrentFrame", &AnimationComponent::CurrentFrame,
            "StartTime", &AnimationComponent::StartTime,
            "IsVerical", &AnimationComponent::IsVerical,
            "IsLooped", &AnimationComponent::IsLooped,
            "Reset",
            [](AnimationComponent &animation)
            {
                animation.CurrentFrame = 0;
                animation.StartTime = SDL_GetTicks();
            }
        );
    }
}
