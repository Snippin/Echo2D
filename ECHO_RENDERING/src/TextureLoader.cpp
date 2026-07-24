#include "Rendering/Essentials/TextureLoader.h"

#include <Logger/Logger.h>

#include <SOIL2/SOIL2.h>

namespace ECHO_RENDERING
{
    std::shared_ptr<Texture> TextureLoader::Create(Texture::TextureType type,
        const std::string &path)
    {
        GLuint id;
        int width;
        int height;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        switch (type)
        {
        case Texture::TextureType::PIXEL:
            LoadTexture(path, width, height, false);
            break;

        case Texture::TextureType::BLENDED:
            LoadTexture(path, width, height, true);
            break;

        case Texture::TextureType::NONE:
            break;

            // TODO: Add other texture types for loading as required

        default:
            assert(false && "The current type is not defined");
            return nullptr;
        }

        return std::make_shared<Texture>(id, width, height, type, path);
    }

    bool TextureLoader::LoadTexture(const std::string &path, int &width,
        int &height, bool blended)
    {
        int channels = 0;

        unsigned char *image = SOIL_load_image(
            path.c_str(),   // File name      -- Image to be loaded
            &width,         // Width          -- Width of image
            &height,        // Height         -- Height of image
            &channels,      // Channels       -- Number of channels
            SOIL_LOAD_AUTO  // Force channels -- Force channel count
        );

        // Check if image is successful
        if (!image)
        {
            auto soil_result = SOIL_last_result();
            ECHO_ERROR("SOIL2 failed to load image [{}] -- {}", path,
                soil_result);
            return false;
        }

        GLint format = GL_RGBA;

        switch (channels)
        {
        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (blended)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }

        glTexImage2D(
            GL_TEXTURE_2D,      // Target           -- Specifies target texture
            0,                  // Level            -- Level of detail. 0 is base image level
            format,             // Internal format  -- Number of color components
            width,              // Width            -- Width of texture image
            height,             // Height           -- Height of texture image
            0,                  // Border
            format,             // Format           -- Format of pixel data
            GL_UNSIGNED_BYTE,   // Type             -- Data type of pixel data
            image               // Data
        );

        // Delete image data from SOIL2
        free(image);

        return true;
    }
}
