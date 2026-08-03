#pragma once

#include "../ECS/Registry.h"

namespace ECHO_CORE::SYSTEMS
{
    class AnimationSystem
    {
    public:
        AnimationSystem(ECHO_CORE::ECS::Registry &registry);
        ~AnimationSystem() = default;

        void Update();

    private:
        ECHO_CORE::ECS::Registry &registry;
    };
}
