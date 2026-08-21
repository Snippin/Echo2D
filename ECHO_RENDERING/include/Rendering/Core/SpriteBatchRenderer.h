#pragma once

#include "../Essentials/BatchType.h"
#include "Batcher.h"

namespace ECHO_RENDERING
{
    class SpriteBatchRenderer : public Batcher<Batch, SpriteGlyph>
    {
    public:
        SpriteBatchRenderer();
        ~SpriteBatchRenderer() = default;

        void End() override;
        void Render() override;

        void AddSprite(const glm::vec4 &sprite_rect, const glm::vec4 &uv_rect,
            GLuint texture_id, int layer = 0, glm::mat4 model = glm::mat4{1.f},
            const Color &color = Color{.R = 255, .G = 255, .B = 255,.A = 255});

    private:
        void Initialise();
        void GenerateBatches() override;
    };
}
