#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ECHO_RENDERING
{
    class Camera2D
    {
    public:
        Camera2D();
        Camera2D(int width, int height);

        glm::vec2 GetPosition() const;
        void SetPosition(const glm::vec2 &new_position);

        float GetScale() const;
        void SetScale(float new_scale);

        glm::mat4 GetCameraMatrix() const;

        void Update();

    private:
        int width;
        int height;
        float scale;

        glm::vec2 position;
        glm::mat4 camera_matrix;
        glm::mat4 ortho_projection;

        bool need_update;
    };
}
