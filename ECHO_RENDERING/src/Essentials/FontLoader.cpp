#include "Rendering/Essentials/FontLoader.h"

#include "Rendering/Essentials/Font.h"

#include <Logger/Logger.h>

#include <fstream>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include <SOIL2/stb_truetype.h>

namespace ECHO_RENDERING
{
    std::shared_ptr<Font> FontLoader::Create(const std::string &path,
        float font_size, int width, int height)
    {
        std::ifstream stream{path, std::ios::binary};
        if (stream.fail())
        {
            ECHO_ERROR("Failed to load font [{}] - Unable to read buffer",
                path);
            return nullptr;
        }

        stream.seekg(0, std::ifstream::end);
        int64_t length = stream.tellg();
        stream.seekg(0, std::ifstream::beg);

        std::vector<unsigned char> buffer;
        buffer.resize(length);

        std::vector<unsigned char> bitmap;
        bitmap.resize(width * height);

        stream.read((char *)(&buffer[0]), length);

        auto data = (void *) new stbtt_bakedchar[96];

        stbtt_BakeFontBitmap(buffer.data(), 0, font_size, bitmap.data(), width,
            height, 32, 96, (stbtt_bakedchar *)(data));

        GLuint font_id;
        glGenTextures(1, &font_id);
        glBindTexture(GL_TEXTURE_2D, font_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
            GL_UNSIGNED_BYTE, bitmap.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return std::make_shared<Font>(font_id, width, height, font_size, data);
    }

    std::shared_ptr<Font> FontLoader::CreateFromMemory(
        const unsigned char *font_data, float font_size, int width, int height)
    {
        auto *bitmap = new unsigned char[width * height];
        auto data = (void *)new stbtt_bakedchar[96];
        stbtt_BakeFontBitmap(font_data, 0, font_size, bitmap, width, height,
            32, 96, (stbtt_bakedchar *)data);

        GLuint font_id;
        glGenTextures(1, &font_id);
        glBindTexture(GL_TEXTURE_2D, font_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
            GL_UNSIGNED_BYTE, bitmap);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        delete[] bitmap;

        return std::make_shared<Font>(font_id, width, height, font_size, data);
    }
}
