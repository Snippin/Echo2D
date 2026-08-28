#pragma once

#include "../Essentials/BatchType.h"
#include "../Essentials/Primitives.h"
#include "Batcher.h"

namespace ECHO_RENDERING
{
    class LineBatchRenderer : public Batcher<LineBatch, LineGlyph>
    {
    public:
        LineBatchRenderer();
        ~LineBatchRenderer() = default;

        void Render() override;

        void AddLine(const Line &line);

    private:
        void GenerateBatches() override;
        void Initialise();
    };
}
