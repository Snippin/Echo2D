#pragma once

#include "../Essentials/BatchType.h"
#include "Batcher.h"

namespace ECHO_RENDERING
{
    struct Circle;

    class CircleBatchRenderer : public Batcher<RectBatch, CircleGlyph>
    {
    public:
        CircleBatchRenderer();
        ~CircleBatchRenderer() = default;

        void Render() override;

        void AddCircle(const glm::vec4 &dest_rect, const Color &color,
            float thickness, const glm::mat4 &model = glm::mat4{1.0f});
        void AddCircle(const Circle &circle);

    private:
        void GenerateBatches() override;
        void Initialise() const;
    };
}
