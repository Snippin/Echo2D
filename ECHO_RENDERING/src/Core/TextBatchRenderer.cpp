#include "Rendering/Core/TextBatchRenderer.h"

namespace ECHO_RENDERING
{
    TextBatchRenderer::TextBatchRenderer() :
        Batcher(false)
    {
        Initialise();
    }

    void TextBatchRenderer::Render()
    {
        if (batches.empty())
        {
            return;
        }

        EnableVAO();

        for (const auto &batch : batches)
        {
            glBindTexture(GL_TEXTURE_2D, batch->FontAtlasID);
            glDrawArrays(GL_TRIANGLES, batch->Offset, batch->NumVertices);
        }

        DisableVAO();
    }

    void TextBatchRenderer::AddText(const std::string &text,
        const std::shared_ptr<Font> &font, const glm::vec2 &position,
        int padding, float wrap, Color color, glm::mat4 model)
    {
        auto glyph = std::make_shared<TextGlyph>(TextGlyph
            {
                .Text = text,
                .Position = position,
                .Color = color,
                .Model = model,
                .Font = font,
                .Padding = padding,
                .Wrap = wrap
            }
        );

        glyphs.push_back(std::move(glyph));
    }

    void TextBatchRenderer::GenerateBatches()
    {
        GLuint offset{0};
        GLuint prev_font_id{0};
        int current_font{0};

        size_t total{0};
        size_t current_vertex{0};

        // Get total characters
        for (const auto &glyph : glyphs)
        {
            total += glyph->Text.size();
        }

        std::vector<Vertex> vertices;
        vertices.resize(total * 6);

        for (const auto &glyph : glyphs)
        {
            std::vector<std::string> chunks{};
            std::string holder{""};
            glm::vec2 temp_pos = glyph->Position;

            if (glyph->Wrap > 1.f)
            {
                // Create text chunk for each line
            }
            else
            {
                chunks.push_back(glyph->Text);
            }

            // Reset text position
            temp_pos = glyph->Position;

            // Add text sprite
            for (const auto &string : chunks)
            {
                for (const auto &character : string)
                {
                    auto letter = glyph->Font->GetGlyph(character, temp_pos);

                    // First triangle
                    vertices[current_vertex++] = Vertex{
                        .Position = glyph->Model * glm::vec4{
                            letter.Min.Position.x,
                            letter.Min.Position.y,
                            0.f, 1.f},
                        .UVs = glm::vec2{letter.Min.UVs.x, letter.Min.UVs.y},
                        .Color = glyph->Color
                    };

                    vertices[current_vertex++] = Vertex{
                        .Position = glyph->Model * glm::vec4{
                            letter.Max.Position.x,
                            letter.Min.Position.y,
                            0.f, 1.f},
                        .UVs = glm::vec2{letter.Max.UVs.x, letter.Min.UVs.y},
                        .Color = glyph->Color
                    };

                    vertices[current_vertex++] = Vertex{
                        .Position = glyph->Model * glm::vec4{
                            letter.Max.Position.x,
                            letter.Max.Position.y,
                            0.f, 1.f},
                        .UVs = glm::vec2{letter.Max.UVs.x, letter.Max.UVs.y},
                        .Color = glyph->Color
                    };

                    // Second triangle
                    vertices[current_vertex++] = Vertex{
                        .Position = glyph->Model * glm::vec4{
                            letter.Min.Position.x,
                            letter.Min.Position.y,
                            0.f, 1.f},
                        .UVs = glm::vec2{letter.Min.UVs.x, letter.Min.UVs.y},
                        .Color = glyph->Color
                    };

                    vertices[current_vertex++] = Vertex{
                        .Position = glyph->Model * glm::vec4{
                            letter.Max.Position.x,
                            letter.Max.Position.y,
                            0.f, 1.f},
                        .UVs = glm::vec2{letter.Max.UVs.x, letter.Max.UVs.y},
                        .Color = glyph->Color
                    };

                    vertices[current_vertex++] = Vertex{
                        .Position = glyph->Model * glm::vec4{
                            letter.Min.Position.x,
                            letter.Max.Position.y,
                            0.f, 1.f},
                        .UVs = glm::vec2{letter.Min.UVs.x, letter.Max.UVs.y},
                        .Color = glyph->Color
                    };

                    if (current_font == 0)
                    {
                        batches.push_back(std::make_shared<TextBatch>(
                            TextBatch{
                                .Offset = offset,
                                .NumVertices = 6,
                                .FontAtlasID = glyph->Font->GetFontAtlasID()
                            }
                        ));
                    }
                    else if (glyph->Font->GetFontAtlasID() != prev_font_id)
                    {
                        batches.push_back(std::make_shared<TextBatch>(
                            TextBatch{
                                .Offset = offset,
                                .NumVertices = 6,
                                .FontAtlasID = glyph->Font->GetFontAtlasID()
                            }
                        ));
                    }
                    else
                    {
                        batches.back()->NumVertices += 6;
                    }

                    current_font++;
                    prev_font_id = glyph->Font->GetFontAtlasID();
                    offset += 6;
                }

                // Move to next line
                temp_pos.x = glyph->Position.x;
                temp_pos.y += glyph->Font->GetFontSize() + glyph->Padding;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, GetVBO());

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
            nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex),
            vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void TextBatchRenderer::Initialise() const
    {
        SetVertexAttribute(0, 2, GL_FLOAT, sizeof(Vertex),
            (void *)offsetof(Vertex, Position));
        SetVertexAttribute(1, 4, GL_UNSIGNED_BYTE, sizeof(Vertex),
            (void *)offsetof(Vertex, Color), GL_TRUE);
        SetVertexAttribute(2, 2, GL_FLOAT, sizeof(Vertex),
            (void *)offsetof(Vertex, UVs));
    }
}
