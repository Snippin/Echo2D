#pragma once

#include "Vertex.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ECHO_RENDERING
{
    struct FontGlyph
    {
        Vertex Min;
        Vertex Max;
    };

    class Font
    {
    public:
        Font(GLuint font_atlas_id, int width, int height, float font_size,
            void *data);
        ~Font();

        inline GLuint GetFontAtlasID() const { return font_atlas_id; }
        inline float GetFontSize() const { return font_size; }

        FontGlyph GetGlyph(char character, glm::vec2 &position) const;

    private:
        GLuint font_atlas_id;
        int width;
        int height;
        float font_size;
        void *data;
    };
}
