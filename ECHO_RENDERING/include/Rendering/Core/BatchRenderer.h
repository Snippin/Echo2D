#pragma once

#include "../Essentials/Vertex.h"

#include <memory>
#include <vector>

namespace ECHO_RENDERING
{
    class BatchRenderer
    {
    public:
        BatchRenderer();
        ~BatchRenderer();

        void Begin();
        void End();
        void Render();

        void AddSprite(const glm::vec4 &sprite_rect, const glm::vec4 &uv_rect,
            GLuint texture_id, int layer = 0, glm::mat4 model = glm::mat4{1.f},
            const Color &color = Color{.R = 255, .G = 255, .B = 255,.A = 255});

    private:
        struct Batch
        {
            GLuint num_indices{0};
            GLuint offset{0};
            GLuint texture_id{0};
        };

        struct Sprite
        {
            Vertex top_left;
            Vertex bot_left;
            Vertex top_right;
            Vertex bot_right;

            int layer;
            GLuint texture_id;
        };

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        std::vector<std::shared_ptr<Sprite>> sprites;
        std::vector<std::shared_ptr<Batch>> batches;

        void Initialise();
        void GenerateBatches();
    };
}
