#pragma once

#include "../ECS/Registry.h"

#include <Rendering/Core/BatchRenderer.h>

namespace ECHO_CORE::SYSTEMS
{
    class RenderSystem
    {
    public:
        RenderSystem(ECHO_CORE::ECS::Registry &registry);
        ~RenderSystem() = default;

        void Render();

    private:
        ECHO_CORE::ECS::Registry &registry;
        std::unique_ptr<ECHO_RENDERING::BatchRenderer> batch_renderer;
    };
}
