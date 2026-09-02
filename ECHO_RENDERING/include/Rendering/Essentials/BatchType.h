#pragma once

#include "Font.h"
#include "Vertex.h"

#include <memory>
#include <string>

namespace ECHO_RENDERING
{
    struct Batch
    {
        GLuint NumIndices{0};
        GLuint Offset{0};
        GLuint TextureId{0};
    };

    struct SpriteGlyph
    {
        Vertex TopLeft;
        Vertex BotLeft;
        Vertex TopRight;
        Vertex BotRight;

        int Layer;
        GLuint TextureId;
    };

    struct LineBatch
    {
        GLuint Offset{2};
        GLuint NumVertices{0};
        float Width{1.f};
    };

    struct LineGlyph
    {
        Vertex P1;
        Vertex P2;
        float Width;
    };

    struct RectBatch
    {
        GLuint NumIndices{0};
        GLuint Offset{0};
    };

    struct RectGlyph
    {
        Vertex TopLeft;
        Vertex BotLeft;
        Vertex TopRight;
        Vertex BotRight;
    };

    struct CircleGlyph
    {
        CircleVertex TopLeft;
        CircleVertex BotLeft;
        CircleVertex TopRight;
        CircleVertex BotRight;
    };

    struct TextBatch
    {
        GLuint Offset{0};
        GLuint NumVertices{0};
        GLuint FontAtlasID{0};
    };

    struct TextGlyph
    {
        std::string Text{""};
        glm::vec2 Position{0.f};
        Color Color{};
        glm::mat4 Model{1.f};
        std::shared_ptr<Font> Font{nullptr};
        int Padding{0};
        float Wrap{0.f};
    };
}
