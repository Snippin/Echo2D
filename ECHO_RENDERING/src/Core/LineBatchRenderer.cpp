#include "Rendering/Core/LineBatchRenderer.h"

#include "Rendering/Essentials/Primitives.h"

namespace ECHO_RENDERING
{
    LineBatchRenderer::LineBatchRenderer()
        : Batcher(false)
    {
        Initialise();
    }

    void LineBatchRenderer::Render()
    {
        glEnable(GL_LINE_SMOOTH);
        EnableVAO();

        for (const auto &batch : batches)
        {
            glDrawArrays(GL_LINES, 0, batch->NumVertices);
        }

        DisableVAO();
        glDisable(GL_LINE_SMOOTH);
    }

    void LineBatchRenderer::AddLine(const Line &line)
    {
        auto new_glyph = std::make_shared<LineGlyph>();
        new_glyph->Width = line.Width;

        new_glyph->P1.Position = line.P1;
        new_glyph->P1.Colour = line.Color;

        new_glyph->P2.Position = line.P2;
        new_glyph->P2.Colour = line.Color;

        glyphs.push_back(std::move(new_glyph));
    }

    void LineBatchRenderer::GenerateBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(glyphs.size() * 2);

        int current_line{0};
        int current_vertex{0};

        for (const auto &line : glyphs)
        {
            if (current_line == 0)
            {
                batches.push_back(std::make_shared<LineBatch>(
                    LineBatch{.Offset = 0,.NumVertices = 2}
                ));
            }
            else
            {
                batches.back()->NumVertices += 2;
            }

            vertices[current_vertex++] = line->P1;
            vertices[current_vertex++] = line->P2;
            batches.back()->Width = line->Width;

            current_line++;
        }

        glBindBuffer(GL_ARRAY_BUFFER, GetVBO());

        // Orphan the current data
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
            nullptr, GL_DYNAMIC_DRAW);
        // Upload new data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex),
            vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void LineBatchRenderer::Initialise() const
    {
        SetVertexAttribute(0, 2, GL_FLOAT, sizeof(Vertex),
            (void *)offsetof(Vertex, Position));
        SetVertexAttribute(1, 4, GL_UNSIGNED_BYTE, sizeof(Vertex),
            (void *)offsetof(Vertex, Colour), GL_TRUE);
    }
}
