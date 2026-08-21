#include "Rendering/Essentials/Texture.h"

namespace ECHO_RENDERING
{
    Texture::Texture() :
        Texture(0, 0, 0, TextureType::NONE)
    {
    }

    Texture::Texture(GLuint id, int width, int height, TextureType type,
        const std::string &tex_path) :
        id{id}, width{width}, height{height}, type{type}, path{tex_path}
    {
    }

    GLuint Texture::GetID() const
    {
        return id;
    }

    int Texture::GetWidth() const
    {
        return width;
    }

    int Texture::GetHeight() const
    {
        return height;
    }

    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
