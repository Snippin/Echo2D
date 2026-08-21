#include "Rendering/Core/SpriteBatchRenderer.h"

#include <algorithm>

namespace ECHO_RENDERING
{
    SpriteBatchRenderer::SpriteBatchRenderer() :
        Batcher(true)
    {
        Initialise();
    }

    void SpriteBatchRenderer::End()
    {
        if (glyphs.empty())
        {
            return;
        }

        // Sort sprites by layer
        std::sort(glyphs.begin(), glyphs.end(),
            [&](const auto &a, const auto &b)
            {
                return a->Layer < b->Layer;
            }
        );

        GenerateBatches();
    }

    void SpriteBatchRenderer::Render()
    {
        if (batches.empty())
        {
            return;
        }

        EnableVAO();

        for (const auto &batch : batches)
        {
            glBindTextureUnit(0, batch->TextureId);
            glDrawElements(GL_TRIANGLES, batch->NumIndices, GL_UNSIGNED_INT,
                (void *)(sizeof(GLuint) * batch->Offset));
        }

        DisableVAO();
    }

    void SpriteBatchRenderer::AddSprite(const glm::vec4 &sprite_rect,
        const glm::vec4 &uv_rect, GLuint texture_id, int layer,
        glm::mat4 model, const Color &color)
    {
        auto new_sprite = std::make_shared<SpriteGlyph>(
            SpriteGlyph
            {
                .TopLeft = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x, sprite_rect.y + sprite_rect.w, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x, uv_rect.y + uv_rect.w},
                    .Colour = color
                },
                .BotLeft = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x, sprite_rect.y, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x, uv_rect.y},
                    .Colour = color
                },
                .TopRight = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x + sprite_rect.z, sprite_rect.y + sprite_rect.w, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w},
                    .Colour = color
                },
                .BotRight = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x + sprite_rect.z, sprite_rect.y, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x + uv_rect.z, uv_rect.y},
                    .Colour = color
                },
                .Layer = layer,
                .TextureId = texture_id
            }
        );

        glyphs.push_back(std::move(new_sprite));
    }

    void SpriteBatchRenderer::Initialise()
    {
        SetVertexAttribute(0, 2, GL_FLOAT, sizeof(Vertex),
            (void *)offsetof(Vertex, Position));
        SetVertexAttribute(1, 2, GL_FLOAT, sizeof(Vertex),
            (void *)offsetof(Vertex, UVs));
        SetVertexAttribute(2, 4, GL_UNSIGNED_BYTE, sizeof(Vertex),
            (void *)offsetof(Vertex, Colour), GL_TRUE);
    }

    void SpriteBatchRenderer::GenerateBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(glyphs.size() * NUM_SPRITE_VERTICES);

        int current_vertex{0};
        int current_sprite{0};
        GLuint offset{0};
        GLuint prev_TextureId{0};

        for (const auto &sprite : glyphs)
        {
            // Is a new sprite
            if (current_sprite == 0)
            {
                batches.emplace_back(std::make_shared<Batch>(Batch{
                    .NumIndices = NUM_SPRITE_INDICES,
                    .Offset = offset,
                    .TextureId = sprite->TextureId
                    }
                ));
            }
            // Sprite is not the same texture
            else if (sprite->TextureId != prev_TextureId)
            {
                batches.emplace_back(std::make_shared<Batch>(Batch{
                    .NumIndices = NUM_SPRITE_INDICES,
                    .Offset = offset,
                    .TextureId = sprite->TextureId
                    }
                ));
            }
            // Is not new and sprite is the same texture
            else
            {
                batches.back()->NumIndices += NUM_SPRITE_INDICES;
            }

            vertices[current_vertex++] = sprite->TopLeft;
            vertices[current_vertex++] = sprite->TopRight;
            vertices[current_vertex++] = sprite->BotRight;
            vertices[current_vertex++] = sprite->BotLeft;

            prev_TextureId = sprite->TextureId;
            offset += NUM_SPRITE_INDICES;
            current_sprite++;
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
}
