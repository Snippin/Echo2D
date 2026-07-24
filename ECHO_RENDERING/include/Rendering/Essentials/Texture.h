#pragma once

#include <glad/glad.h>

#include <string>

namespace ECHO_RENDERING
{
    class Texture
    {
    public:
        enum class TextureType
        {
            PIXEL = 0,
            BLENDED,
            NONE,
        };

        Texture();
        Texture(GLuint id, int width, int height,
            TextureType type = TextureType::PIXEL,
            const std::string &tex_path = "");
        ~Texture() = default;

        GLuint GetID() const;
        int GetWidth() const;
        int GetHeight() const;

        void Bind();
        void Unbind();

    private:
        GLuint id;
        int width;
        int height;
        TextureType type;
        std::string path;
    };
}
