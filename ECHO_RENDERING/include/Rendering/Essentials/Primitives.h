#pragma once

#include "Font.h"
#include "Vertex.h"

#include <memory>
#include <string>

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
        float Radius{0.f};
        float Thickness{1.f};
        Color Color{};
    };

    struct Text
    {
        glm::vec2 Position{0.f};
        std::string String{""};
        int Padding{4};
        float Wrap{0.f};
        Color Color{};
        std::shared_ptr<Font> Font{nullptr};
    };
}
