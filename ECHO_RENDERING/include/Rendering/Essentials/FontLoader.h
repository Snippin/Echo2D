#pragma once

#include <memory>
#include <string>

namespace ECHO_RENDERING
{
    class Font;

    class FontLoader
    {
    public:
        FontLoader() = delete;

        static std::shared_ptr<Font> Create(const std::string &path,
            float font_size = 32.f, int width = 512, int height = 512);
        static std::shared_ptr<Font> CreateFromMemory(
            const unsigned char *font_data, float font_size = 32.f,
            int width = 512, int height = 512);
    };
}
