#pragma once

#include "SoundParams.h"

#include <EchoUtilities/SDLWrapper.h>

namespace ECHO_SOUNDS
{
    class SoundFX
    {
    public:
        SoundFX(const SoundParams &params, SoundFXPtr sound_fx);
        ~SoundFX() = default;

        Mix_Chunk *GetSoundFXPtr() const;
        const std::string &GetName() const;
        const std::string &GetDescription() const;
        const std::string &GetFilePath() const;
        double GetDuration() const;

    private:
        SoundParams params;
        SoundFXPtr sound_fx;
    };
}
