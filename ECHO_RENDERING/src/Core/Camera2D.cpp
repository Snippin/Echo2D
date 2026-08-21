#include "Rendering/Core/Camera2D.h"

namespace ECHO_RENDERING
{
    Camera2D::Camera2D() :
        Camera2D(640, 480)
    {
    }

    Camera2D::Camera2D(int width, int height) :
        width{width}, height{height}, scale{1.f},
        position{glm::vec2{0.f}}, camera_matrix{1.f},
        ortho_projection{1.f}, need_update{true}
    {
        float half_width = static_cast<float>(width) * 0.5f;
        float half_height = static_cast<float>(height) * 0.5f;

        // Flip top and bottom values due to image being rendered upside down
        ortho_projection = glm::ortho(
            -half_width,    // Left
            half_width,     // Right
            half_height,    // Bottom
            -half_height,   // Top
            -1.f,           // Near
            1.f             // Far
        );

        Update();
    }

    glm::vec2 Camera2D::GetPosition() const
    {
        return position;
    }

    void Camera2D::SetPosition(glm::vec2 new_position)
    {
        position = new_position;
        need_update = true;
    }

    float Camera2D::GetScale() const
    {
        return scale;
    }

    void Camera2D::SetScale(float new_scale)
    {
        scale = new_scale;
        need_update = true;
    }

    glm::mat4 Camera2D::GetCameraMatrix() const
    {
        return camera_matrix;
    }

    void Camera2D::Update()
    {
        if (!need_update)
        {
            return;
        }

        // Translate
        glm::vec3 translate{-position.x, -position.y, 0.f};
        camera_matrix = glm::translate(ortho_projection, translate);

        // Scale
        glm::vec3 scaling{scale, scale, 0.f};
        camera_matrix *= glm::scale(glm::mat4{1.f}, scaling);

        need_update = false;
    }
}
