#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ECHO_RENDERING
{
    struct Color
    {
        GLubyte R = 255;
        GLubyte G = 255;
        GLubyte B = 255;
        GLubyte A = 255;
    };

    struct Vertex
    {
        glm::vec2 Position{0.f};
        glm::vec2 UVs{0.f};
        Color Colour{.R = 255, .G = 255, .B = 255, .A = 255};

        void SetColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
        {
            Colour.R = r;
            Colour.G = g;
            Colour.B = b;
            Colour.A = a;
        }

        void SetColour(GLuint new_color)
        {
            // GLuint is a 32-bit integer. Treat it as 4 packed bytes: 0xRRGGBBAA
            // Each color component (R, G, B, A) is an 8-bit value (0 to 255).
            // Shift target byte all the way to the right (the lowest 8 bits)
            // `0xFF` is a mask that keep the lowest 8 bits and turn all higher bits into 0
            // Shift the colored byte down to lowest 8 bits and isolate it with the mask
            Colour.R = (new_color >> 24) & 0xFF;
            Colour.G = (new_color >> 16) & 0xFF;
            Colour.B = (new_color >> 8) & 0xFF;
            Colour.A = (new_color >> 0) & 0xFF;
        }
    };
}
