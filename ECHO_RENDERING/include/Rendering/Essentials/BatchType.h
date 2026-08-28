#pragma once

#include "Vertex.h"

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
}
