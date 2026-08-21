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
}
