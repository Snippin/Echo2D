#include "Rendering/Core/Renderer.h"

#include "Rendering/Core/Camera2D.h"
#include "Rendering/Essentials/Shader.h"

namespace ECHO_RENDERING
{
    Renderer::Renderer() :
        line_renderer{nullptr}, sprite_renderer{nullptr}
    {
        line_renderer = std::make_unique<LineBatchRenderer>();
        rect_renderer = std::make_unique<RectBatchRenderer>();
        sprite_renderer = std::make_unique<SpriteBatchRenderer>();
    }

    void Renderer::SetClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
        const
    {
        glClearColor(r, g, b, a);
    }

    void Renderer::ClearBuffers(bool color, bool depth, bool stencil) const
    {
        glClear(
            (color ? GL_COLOR_BUFFER_BIT : 0) |
            (depth ? GL_DEPTH_BUFFER_BIT : 0) |
            (stencil ? GL_STENCIL_BUFFER_BIT : 0)
        );
    }

    void Renderer::SetCapability(GLCapability capability, bool enable)
    {
        if (enable)
        {
            glEnable(static_cast<GLenum>(capability));
        }
        else
        {
            glDisable(static_cast<GLenum>(capability));
        }
    }

    bool Renderer::IsCapabilityEnabled(GLCapability capability) const
    {
        return glIsEnabled(static_cast<GLenum>(capability));
    }

    void Renderer::SetBlendCapability(BlendingFactors s_factor,
        BlendingFactors d_factor) const
    {
        glBlendFunc(
            static_cast<GLenum>(s_factor),
            static_cast<GLenum>(d_factor)
        );
    }

    void Renderer::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
        const
    {
        glViewport(x, y, width, height);
    }

    void Renderer::DrawLine(const Line &line)
    {
        lines.push_back(line);
    }

    void Renderer::DrawLine(const glm::vec2 &p1, const glm::vec2 &p2,
        const Color &color, float width)
    {
        lines.push_back(
            Line{
                .P1 = p1,
                .P2 = p2,
                .Width = width,
                .Color = color
            }
        );
    }

    void Renderer::DrawRect(const Rect &rect)
    {
        glm::vec2 offset{rect.Width * 0.5f, rect.Height * 0.5f};

        // Top
        DrawLine(Line{
            .P1 = rect.Position + glm::vec2{-offset.x, -offset.y},
            .P2 = rect.Position + glm::vec2{offset.x, -offset.y},
            .Color = rect.Color
        });

        // Bottom
        DrawLine(Line{
            .P1 = rect.Position + glm::vec2{-offset.x, offset.y},
            .P2 = rect.Position + glm::vec2{offset.x, offset.y},
            .Color = rect.Color
        });

        // Left
        DrawLine(Line{
            .P1 = rect.Position + glm::vec2{-offset.x, -offset.y},
            .P2 = rect.Position + glm::vec2{-offset.x, offset.y},
            .Color = rect.Color
        });

        // Right
        DrawLine(Line{
            .P1 = rect.Position + glm::vec2{offset.x, -offset.y},
            .P2 = rect.Position + glm::vec2{offset.x, offset.y},
            .Color = rect.Color
        });
    }

    void Renderer::DrawRect(const glm::vec2 &position, float width,
        float height, const Color &color)
    {
        DrawRect(Rect{
            .Position = position,
            .Width = width,
            .Height = height,
            .Color = color
        });
    }

    void Renderer::DrawFilledRect(const Rect &rect)
    {
        rects.push_back(rect);
    }

    void Renderer::DrawCircle(const Circle &circle)
    {
        circles.push_back(circle);
    }

    void Renderer::DrawCircle(const glm::vec2 &position, float radius,
        const Color &color, float thickness)
    {
        circles.push_back(Circle{
            .Position = position,
            .Thickness = thickness,
            .Radius = radius,
            .Color = color
        });
    }

    void Renderer::DrawLines(Shader &shader, const Camera2D &camera)
    {
        if (lines.empty())
        {
            return;
        }

        auto camera_matrix = camera.GetCameraMatrix();
        shader.Enable();
        shader.SetUniformMat4("uProjection", camera_matrix);

        line_renderer->Begin();
        for (const auto &line : lines)
        {
            line_renderer->AddLine(line);
        }
        line_renderer->End();
        line_renderer->Render();
        shader.Disable();
    }

    void Renderer::DrawFilledRects(Shader &shader, const Camera2D &camera)
    {
        if (rects.empty())
        {
            return;
        }

        auto camera_matrix = camera.GetCameraMatrix();
        shader.Enable();
        shader.SetUniformMat4("uProjection", camera_matrix);

        rect_renderer->Begin();
        for (const auto &rect : rects)
        {
            rect_renderer->AddRect(rect);
        }
        rect_renderer->End();
        rect_renderer->Render();
        shader.Disable();
    }

    void Renderer::DrawCircles(Shader &shader, const Camera2D &camera)
    {
    }

    void Renderer::ClearPrimitives()
    {
        lines.clear();
        rects.clear();
        circles.clear();
    }
}
