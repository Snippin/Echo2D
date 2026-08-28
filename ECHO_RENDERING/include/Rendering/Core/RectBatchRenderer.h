#pragma once

#include "../Essentials/BatchType.h"
#include "Batcher.h"

namespace ECHO_RENDERING
{
    struct Rect;

    class RectBatchRenderer : public Batcher<RectBatch, RectGlyph>
    {
    public:
        RectBatchRenderer();
        ~RectBatchRenderer() = default;

        void Render() override;

        void AddRect(const glm::vec4 &dest_rect, const Color &color);
        void AddRect(const Rect &rect);

    private:
        void GenerateBatches() override;
        void Initialise() const;
    };
}
