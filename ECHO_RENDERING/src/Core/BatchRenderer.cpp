#include "Rendering/Core/BatchRenderer.h"

#include <algorithm>

namespace ECHO_RENDERING
{
    constexpr size_t NUM_SPRITE_VERTICES = 4;
    constexpr size_t NUM_SPRITE_INDICES = 6;
    constexpr size_t MAX_SPRITES = 10000;
    constexpr size_t MAX_INDICES = MAX_SPRITES * NUM_SPRITE_INDICES;
    constexpr size_t MAX_VERTICES = MAX_SPRITES * NUM_SPRITE_VERTICES;

    BatchRenderer::BatchRenderer() :
        VAO{0}, VBO{0}, EBO{0}, sprites{}, batches{}
    {
        Initialise();
    }

    BatchRenderer::~BatchRenderer()
    {
        if (VAO)
        {
            glDeleteVertexArrays(1, &VAO);
        }

        if (VBO)
        {
            glDeleteBuffers(1, &VBO);
        }

        if (EBO)
        {
            glDeleteBuffers(1, &EBO);
        }
    }

    void BatchRenderer::Begin()
    {
        sprites.clear();
        batches.clear();
    }

    void BatchRenderer::End()
    {
        if (sprites.empty())
        {
            return;
        }

        // Sort sprites by layer
        std::sort(sprites.begin(), sprites.end(),
            [&](const auto &a, const auto &b)
            {
                return a->layer < b->layer;
            }
        );

        GenerateBatches();
    }

    void BatchRenderer::Render()
    {
        if (batches.empty())
        {
            return;
        }

        glBindVertexArray(VAO);

        for (const auto &batch : batches)
        {
            glBindTextureUnit(0, batch->texture_id);
            glDrawElements(GL_TRIANGLES, batch->num_indices, GL_UNSIGNED_INT,
                (void *)(sizeof(GLuint) * batch->offset));
        }

        glBindVertexArray(0);
    }

    void BatchRenderer::AddSprite(const glm::vec4 &sprite_rect,
        const glm::vec4 &uv_rect, GLuint texture_id, int layer,
        glm::mat4 model, const Color &color)
    {
        auto new_sprite = std::make_shared<Sprite>(
            Sprite
            {
                .top_left = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x, sprite_rect.y + sprite_rect.w, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x, uv_rect.y + uv_rect.w},
                    .Colour = color
                },
                .bot_left = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x, sprite_rect.y, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x, uv_rect.y},
                    .Colour = color
                },
                .top_right = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x + sprite_rect.z, sprite_rect.y + sprite_rect.w, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w},
                    .Colour = color
                },
                .bot_right = Vertex{
                    .Position = model * glm::vec4(sprite_rect.x + sprite_rect.z, sprite_rect.y, 0.f, 1.f),
                    .UVs = glm::vec2{uv_rect.x + uv_rect.z, uv_rect.y},
                    .Colour = color
                },
                .layer = layer,
                .texture_id = texture_id
            }
        );

        sprites.push_back(std::move(new_sprite));
    }

    void BatchRenderer::Initialise()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr,
            GL_DYNAMIC_DRAW);

        // Generate index buffer
        GLuint offset{0};
        GLuint indices[NUM_SPRITE_INDICES]{0, 1, 2, 2, 3, 0};

        auto indices_array = std::make_unique<GLuint[]>(MAX_INDICES);

        // Loop through the index array in chunks of `NUM_SPRITE_INDICES` (6)
        for (size_t i = 0; i < MAX_INDICES; i += NUM_SPRITE_INDICES)
        {
            // Copy base triangle pattern into the current chunk of the array
            // shifting the vertex references by the current `offset`
            for (size_t j = 0; j < NUM_SPRITE_INDICES; j++)
            {
                indices_array[i + j] = indices[j] + offset;
            }

            // Advance the offset by the number of vertices in a sprite (4)
            offset += NUM_SPRITE_VERTICES;
        }

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * MAX_INDICES,
            indices_array.get(), GL_DYNAMIC_DRAW);

        // Position
        glVertexAttribPointer(
            0,					                // Attribute    -- Layout position in the shader
            2,					                // Size		    -- Number of components per vertex
            GL_FLOAT,			                // Type		    -- Data type of components
            GL_FALSE,			                // Normalized   -- Specifies if fixed-point data values should be normalized
            sizeof(Vertex),	                    // Stride       -- Specifies byte offset between consecutive attributes
            (void *)offsetof(Vertex, Position) 	// Pointer      -- Specifies the offset of the first component
        );

        // UVs
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void *)offsetof(Vertex, UVs)   // Offset of positional data to the first UV coord
        );

        // Colour
        glVertexAttribPointer(
            2,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            sizeof(Vertex),
            (void *)offsetof(Vertex, Colour)    // Offset of positional data to the first UV coord
        );

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void BatchRenderer::GenerateBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(sprites.size() * NUM_SPRITE_VERTICES);

        int current_vertex{0};
        int current_sprite{0};
        GLuint offset{0};
        GLuint prev_texture_id{0};

        for (const auto &sprite : sprites)
        {
            // Is a new sprite
            if (current_sprite == 0)
            {
                batches.emplace_back(std::make_shared<Batch>(Batch{
                    .num_indices = NUM_SPRITE_INDICES,
                    .offset = offset,
                    .texture_id = sprite->texture_id
                    }
                ));
            }
            // Sprite is not the same texture
            else if (sprite->texture_id != prev_texture_id)
            {
                batches.emplace_back(std::make_shared<Batch>(Batch{
                    .num_indices = NUM_SPRITE_INDICES,
                    .offset = offset,
                    .texture_id = sprite->texture_id
                    }
                ));
            }
            // Is not new and sprite is the same texture
            else
            {
                batches.back()->num_indices += NUM_SPRITE_INDICES;
            }

            vertices[current_vertex++] = sprite->top_left;
            vertices[current_vertex++] = sprite->top_right;
            vertices[current_vertex++] = sprite->bot_right;
            vertices[current_vertex++] = sprite->bot_left;

            prev_texture_id = sprite->texture_id;
            offset += NUM_SPRITE_INDICES;
            current_sprite++;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Orphan the current data
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
            nullptr, GL_DYNAMIC_DRAW);
        // Upload new data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex),
            vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
