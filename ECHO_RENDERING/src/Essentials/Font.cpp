#include "Rendering/Essentials/Font.h"

#include <SOIL2/stb_truetype.h>

namespace ECHO_RENDERING
{
    Font::Font(GLuint font_atlas_id, int width, int height, float font_size,
        void *data) :
        font_atlas_id{font_atlas_id}, width{width}, height{height},
        font_size{font_size}, data{data}
    {
    }

    Font::~Font()
    {
        if (font_atlas_id != 0)
        {
            glDeleteTextures(1, &font_atlas_id);
        }

        if (data)
        {
            delete data;
        }
    }

    FontGlyph Font::GetGlyph(char character, glm::vec2 &position) const
    {
        FontGlyph glyph{};

        if (character >= 32 && character < 128)
        {
            stbtt_aligned_quad quad;
            stbtt_GetBakedQuad(
                (stbtt_bakedchar *)(data),
                width,
                height,
                character - 32,
                &position.x,
                &position.y,
                &quad,
                1
            );

            glyph.Min = Vertex{
                .Position = glm::vec2{quad.x0, quad.y0},
                .UVs = glm::vec2{quad.s0, quad.t0}
            };
            glyph.Max = Vertex{
                .Position = glm::vec2{quad.x1, quad.y1},
                .UVs = glm::vec2{quad.s1, quad.t1}
            };
        }

        return glyph;
    }
}
