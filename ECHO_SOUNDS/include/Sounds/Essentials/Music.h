#pragma once

#include "SoundParams.h"

#include <EchoUtilities/SDLWrapper.h>

namespace ECHO_SOUNDS
{
    class Music
    {
    public:
        Music(const SoundParams &params, MusicPtr music);
        ~Music() = default;

        Mix_Music *GetMusicPtr() const;
        const std::string &GetName() const;
        const std::string &GetDescription() const;
        const std::string &GetFilePath() const;
        double GetDuration() const;

    private:
        SoundParams params{};
        MusicPtr music{};
    };
}
