#pragma once

#include "../Essentials/BatchType.h"
#include "Batcher.h"

namespace ECHO_RENDERING
{
    class RectBatchRenderer : public Batcher<RectBatch, RectGlyph>
    {
    public:
        RectBatchRenderer();
        ~RectBatchRenderer() = default;

        void End() override;
        void Render() override;

        void AddRect(const glm::vec4 &dest_rect, const Color &color);
        void AddRect(const struct Rect &rect);

    private:
        void GenerateBatches() override;
        void Initialise() const;
    };
}
