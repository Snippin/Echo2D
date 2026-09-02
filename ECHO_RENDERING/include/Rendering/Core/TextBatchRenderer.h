#pragma once

#include "../Essentials/BatchType.h"
#include "Batcher.h"

namespace ECHO_RENDERING
{
    class TextBatchRenderer : public Batcher<TextBatch, TextGlyph>
    {
    public:
        TextBatchRenderer();
        ~TextBatchRenderer() = default;

        void Render() override;

        void AddText(const std::string &text,
            const std::shared_ptr<Font> &font,
            const glm::vec2 &position, int padding = 4,
            float wrap = 0.f, Color color = Color{},
            glm::mat4 model = glm::mat4{1.f});

    private:
        void GenerateBatches() override;
        void Initialise() const;
    };
}
