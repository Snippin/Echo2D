#pragma once

#include <string>

namespace ECHO_SOUNDS
{
    struct SoundParams
    {
        std::string Name{""};
        std::string Description{""};
        std::string FilePath{""};
        double Duration{0};
    };
}
