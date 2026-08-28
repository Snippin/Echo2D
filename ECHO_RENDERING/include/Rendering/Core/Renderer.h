#pragma once

#include "../Essentials/Primitives.h"
#include "LineBatchRenderer.h"
#include "RectBatchRenderer.h"
#include "SpriteBatchRenderer.h"

#include <glad/glad.h>
#include <memory>

namespace ECHO_RENDERING
{
    class Shader;
    class Camera2D;

    class Renderer
    {
    public:
        enum class GLCapability : GLuint
        {
            BLEND = GL_BLEND,
            DEPTH_TEST = GL_DEPTH_TEST,
            LINE_SMOOTH = GL_LINE_SMOOTH,
        };

        enum class BlendingFactors : GLuint
        {
            ONE = GL_ONE,
            SRC_ALPHA = GL_SRC_ALPHA,
            ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        };

        Renderer();
        ~Renderer() = default;

        // Set color buffers to screen upon them being cleared
        // Values are clamped between 0 - 1
        void SetClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) const;
        void ClearBuffers(bool color = true, bool depth = true,
            bool stencil = false) const;
        void SetCapability(GLCapability capability, bool enable);
        bool IsCapabilityEnabled(GLCapability capability) const;
        void SetBlendCapability(BlendingFactors s_factor,
            BlendingFactors d_factor) const;
        void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
            const;

        // Primitive draw calls
        void DrawLine(const Line &line);
        void DrawLine(const glm::vec2 &p1, const glm::vec2 &p2,
            const Color &color, float width = 1.f);

        void DrawRect(const Rect &rect);
        void DrawRect(const glm::vec2 &position, float width, float height,
            const Color &color);
        void DrawFilledRect(const Rect &rect);

        void DrawCircle(const Circle &circle);
        void DrawCircle(const glm::vec2 &position, float radius,
            const Color &color, float thickness = 1.f);

        void DrawLines(Shader &shader, const Camera2D &camera);
        void DrawFilledRects(Shader &shader, const Camera2D &camera);
        void DrawCircles(Shader &shader, const Camera2D &camera);
        void ClearPrimitives();

    private:
        std::vector<Line> lines;
        std::vector<Rect> rects;
        std::vector<Circle> circles;

        std::unique_ptr<LineBatchRenderer> line_renderer;
        std::unique_ptr<RectBatchRenderer> rect_renderer;
        std::unique_ptr<SpriteBatchRenderer> sprite_renderer;
    };
}
