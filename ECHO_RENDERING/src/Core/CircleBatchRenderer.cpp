#include "Rendering/Core/CircleBatchRenderer.h"

#include "Rendering/Essentials/Primitives.h"

namespace ECHO_RENDERING
{
    CircleBatchRenderer::CircleBatchRenderer() :
        Batcher(true)
    {
        Initialise();
    }

    void CircleBatchRenderer::Render()
    {
        if (batches.empty())
        {
            return;
        }

        EnableVAO();

        for (const auto &batch : batches)
        {
            glDrawElements(GL_TRIANGLES, batch->NumIndices, GL_UNSIGNED_INT,
                (void *)(sizeof(GLuint) * batch->Offset));
        }

        DisableVAO();
    }

    void CircleBatchRenderer::AddCircle(const glm::vec4 &dest_rect,
        const Color &color, float thickness, const glm::mat4 &model)
    {
        auto half_size = glm::vec2{dest_rect.z * 0.5f, dest_rect.w * 0.5f};

        auto new_circle = std::make_shared<CircleGlyph>(CircleGlyph{
            .TopLeft = CircleVertex {
                .Position = model * glm::vec4{
                    dest_rect.x - half_size.x,
                    dest_rect.y - half_size.y,
                    0.f, 1.f
                },
                .UVs = glm::vec2{-1.f, 1.f},
                .Colour = color,
                .Thickness = thickness
            },
            .BotLeft = CircleVertex {
                .Position = model * glm::vec4{
                    dest_rect.x - half_size.x,
                    dest_rect.y + half_size.y,
                    0.f, 1.f
                },
                .UVs = glm::vec2{-1.f, -1.f},
                .Colour = color,
                .Thickness = thickness
            },
            .TopRight = CircleVertex {
                .Position = model * glm::vec4{
                    dest_rect.x + half_size.x,
                    dest_rect.y - half_size.y,
                    0.f, 1.f
                },
                .UVs = glm::vec2{1.f, 1.f},
                .Colour = color,
                .Thickness = thickness
            },
            .BotRight = CircleVertex {
                .Position = model * glm::vec4{
                    dest_rect.x + half_size.x,
                    dest_rect.y + half_size.y,
                    0.f, 1.f
                },
                .UVs = glm::vec2{1.f, -1.f},
                .Colour = color,
                .Thickness = thickness
            },
        });

        glyphs.emplace_back(new_circle);
    }

    void CircleBatchRenderer::AddCircle(const Circle &circle)
    {
        auto model = glm::mat4{1.f};

        auto new_circle = std::make_shared<CircleGlyph>(CircleGlyph{
            .TopLeft = CircleVertex {
                .Position = model * glm::vec4{
                    circle.Position.x - circle.Radius,
                    circle.Position.y - circle.Radius,
                    0.f, 1.f
                },
                .UVs = glm::vec2{-1.f, 1.f},
                .Colour = circle.Color,
                .Thickness = circle.Thickness
            },
            .BotLeft = CircleVertex {
                .Position = model * glm::vec4{
                    circle.Position.x - circle.Radius,
                    circle.Position.y + circle.Radius,
                    0.f, 1.f
                },
                .UVs = glm::vec2{-1.f, -1.f},
                .Colour = circle.Color,
                .Thickness = circle.Thickness
            },
            .TopRight = CircleVertex {
                .Position = model * glm::vec4{
                    circle.Position.x + circle.Radius,
                    circle.Position.y - circle.Radius,
                    0.f, 1.f
                },
                .UVs = glm::vec2{1.f, 1.f},
                .Colour = circle.Color,
                .Thickness = circle.Thickness
            },
            .BotRight = CircleVertex {
                .Position = model * glm::vec4{
                    circle.Position.x + circle.Radius,
                    circle.Position.y + circle.Radius,
                    0.f, 1.f
                },
                .UVs = glm::vec2{1.f, -1.f},
                .Colour = circle.Color,
                .Thickness = circle.Thickness
            },
        });

        glyphs.emplace_back(new_circle);
    }

    void CircleBatchRenderer::GenerateBatches()
    {
        std::vector<CircleVertex> vertices;
        vertices.resize(glyphs.size() * NUM_SPRITE_VERTICES);

        int current_circle{0};
        int current_vertex{0};
        GLuint offset{0};

        for (const auto &circle : glyphs)
        {
            if (current_circle == 0)
            {
                batches.emplace_back(std::make_shared<RectBatch>(RectBatch{
                    .NumIndices = NUM_SPRITE_INDICES, .Offset = offset}));
            }
            else
            {
                batches.back()->NumIndices += NUM_SPRITE_INDICES;
            }

            vertices[current_vertex++] = circle->TopLeft;
            vertices[current_vertex++] = circle->TopRight;
            vertices[current_vertex++] = circle->BotRight;
            vertices[current_vertex++] = circle->BotLeft;

            offset += NUM_SPRITE_INDICES;
            current_circle++;
        }

        glBindBuffer(GL_ARRAY_BUFFER, GetVBO());

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(CircleVertex),
            nullptr, GL_DYNAMIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0,
            vertices.size() * sizeof(CircleVertex), vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void CircleBatchRenderer::Initialise() const
    {
        SetVertexAttribute(0, 2, GL_FLOAT, sizeof(CircleVertex),
            (void *)offsetof(CircleVertex, Position));
        SetVertexAttribute(1, 2, GL_FLOAT, sizeof(CircleVertex),
            (void *)offsetof(CircleVertex, UVs));
        SetVertexAttribute(2, 4, GL_UNSIGNED_BYTE, sizeof(CircleVertex),
            (void *)offsetof(CircleVertex, Colour), GL_TRUE);
        SetVertexAttribute(3, 1, GL_FLOAT, sizeof(CircleVertex),
            (void *)offsetof(CircleVertex, Thickness));
    }
}
