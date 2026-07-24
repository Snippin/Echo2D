#pragma once

#include "Texture.h"

#include <memory>

namespace ECHO_RENDERING
{
    class TextureLoader
    {
    public:
        TextureLoader() = delete;

        static std::shared_ptr<Texture> Create(Texture::TextureType type,
            const std::string &path);

    private:
        static bool LoadTexture(const std::string &path, int &width,
            int &height, bool blended = false);
    };
}
