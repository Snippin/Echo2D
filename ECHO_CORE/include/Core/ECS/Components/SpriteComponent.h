#pragma once

#include "../Registry.h"

#include <Rendering/Essentials/Vertex.h>

#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    struct UVs
    {
        float X{0.f};
        float Y{0.f};
        float UVWidth{0.f};
        float UVHeight{0.f};
    };

    struct SpriteComponent
    {
        float Width{0.f};
        float Height{0.f};
        UVs Uvs{.X = 0.f, .Y = 0.f, .UVWidth = 0.f, .UVHeight = 0.f};

        ECHO_RENDERING::Color Color{.R = 255, .G = 255, .B = 255, .A = 255};
        int StartX{0};
        int StartY{0};
        int Layer{0};
        float Rotation{0};
        bool IsHidden{};

        std::string TextureName{""};

        void GenerateUVs(int texture_width, int texture_height)
        {
            Uvs.UVWidth = Width / texture_width;
            Uvs.UVHeight = Height / texture_height;

            Uvs.X = StartX * Uvs.UVWidth;
            Uvs.Y = StartY * Uvs.UVHeight;
        }

        static void CreateLuaBind(sol::state &lua,
            ECHO_CORE::ECS::Registry &registry);
    };
}
