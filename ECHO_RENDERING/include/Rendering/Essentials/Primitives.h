#pragma once

#include "Vertex.h"

namespace ECHO_RENDERING
{
    struct Line
    {
        glm::vec2 P1{0.f};
        glm::vec2 P2{0.f};
        float Width{1.f};
        Color Color{};
    };

    struct Rect
    {
        glm::vec2 Position{0.f};
        float Width{0.f};
        float Height{0.f};
        Color Color{};
    };

    struct Circle
    {
        glm::vec2 Position{0.f};
        float thickness{1.f};
        float radius{0.f};
        Color Color{};
    };
}
