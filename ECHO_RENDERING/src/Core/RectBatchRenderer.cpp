#include "Rendering/Core/RectBatchRenderer.h"

#include "Rendering/Essentials/Primitives.h"

namespace ECHO_RENDERING
{
    RectBatchRenderer::RectBatchRenderer() :
        Batcher(true)
    {
        Initialise();
    }

    void RectBatchRenderer::Render()
    {
        EnableVAO();

        for (const auto &batch : batches)
        {
            glDrawElements(GL_TRIANGLES, batch->NumIndices, GL_UNSIGNED_INT,
                (void *)(sizeof(GLuint) * batch->Offset));
        }

        DisableVAO();
    }

    void RectBatchRenderer::AddRect(const glm::vec4 &dest_rect,
        const Color &color)
    {
        auto half_size = glm::vec2{dest_rect.z * 0.5f, dest_rect.w * 0.5f};

        auto new_glyph = std::make_shared<RectGlyph>();

        new_glyph->TopLeft.Colour = color;
        new_glyph->TopLeft.Position =
            glm::vec2{dest_rect.x - half_size.x, dest_rect.y - half_size.y};

        new_glyph->BotLeft.Colour = color;
        new_glyph->BotLeft.Position =
            glm::vec2{dest_rect.x - half_size.x, dest_rect.y + half_size.y};

        new_glyph->TopRight.Colour = color;
        new_glyph->TopRight.Position =
            glm::vec2{dest_rect.x + half_size.x, dest_rect.y - half_size.y};

        new_glyph->BotRight.Colour = color;
        new_glyph->BotRight.Position =
            glm::vec2{dest_rect.x + half_size.x, dest_rect.y + half_size.y};

        glyphs.push_back(std::move(new_glyph));
    }

    void RectBatchRenderer::AddRect(const Rect &rect)
    {
        auto half_size = glm::vec2{rect.Width * 0.5f, rect.Height * 0.5f};

        auto new_glyph = std::make_shared<RectGlyph>();

        new_glyph->TopLeft.Colour = rect.Color;
        new_glyph->TopLeft.Position =
            rect.Position + glm::vec2{-half_size.x, -half_size.y};

        new_glyph->BotLeft.Colour = rect.Color;
        new_glyph->BotLeft.Position =
            rect.Position + glm::vec2{-half_size.x, half_size.y};

        new_glyph->TopRight.Colour = rect.Color;
        new_glyph->TopRight.Position =
            rect.Position + glm::vec2{half_size.x, -half_size.y};

        new_glyph->BotRight.Colour = rect.Color;
        new_glyph->BotRight.Position =
            rect.Position + glm::vec2{half_size.x, half_size.y};

        glyphs.push_back(std::move(new_glyph));
    }

    void RectBatchRenderer::GenerateBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(glyphs.size() * 4);

        int current_vertex{0};

        batches.push_back(std::make_shared<RectBatch>(
            RectBatch{.NumIndices = 6, .Offset = 0}
        ));

        for (const auto &rect : glyphs)
        {
            batches.back()->NumIndices += 6;

            vertices[current_vertex++] = rect->TopLeft;
            vertices[current_vertex++] = rect->TopRight;
            vertices[current_vertex++] = rect->BotRight;
            vertices[current_vertex++] = rect->BotLeft;
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

    void RectBatchRenderer::Initialise() const
    {
        SetVertexAttribute(0, 2, GL_FLOAT, sizeof(Vertex),
            (void *)offsetof(Vertex, Position));
        SetVertexAttribute(1, 4, GL_UNSIGNED_BYTE, sizeof(Vertex),
            (void *)offsetof(Vertex, Colour), GL_TRUE);
    }
}
