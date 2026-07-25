#pragma once

#include <Rendering/Essentials/Vertex.h>

namespace ECHO_CORE::ECS
{
    struct UVs
    {
        float X{0.f};
        float Y{0.f};
        float UV_Width{0.f};
        float UV_Height{0.f};
    };

    struct SpriteComponent
    {
        float Width{0.f};
        float Height{0.f};
        UVs Uvs{.X = 0.f, .Y = 0.f, .UV_Width = 0.f, .UV_Height = 0.f};

        ECHO_RENDERING::Color Colour{.R = 255, .G = 255, .B = 255, .A = 255};
        int Start_X{0};
        int Start_Y{0};
        int Layer{0};

        std::string Texture_Name{""};

        void GenerateUVs(int texture_width, int texture_height)
        {
            Uvs.UV_Width = Width / texture_width;
            Uvs.UV_Height = Height / texture_height;

            Uvs.X = Start_X * Uvs.UV_Width;
            Uvs.Y = Start_Y * Uvs.UV_Height;
        }
    };
}
