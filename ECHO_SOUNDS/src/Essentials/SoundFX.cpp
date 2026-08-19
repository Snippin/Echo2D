#include "Sounds/Essentials/SoundFX.h"

namespace ECHO_SOUNDS
{
    SoundFX::SoundFX(const SoundParams &params, SoundFXPtr sound_fx) :
        params{params}, sound_fx{std::move(sound_fx)}
    {
    }

    Mix_Chunk *SoundFX::GetSoundFXPtr() const
    {
        if (!sound_fx)
        {
            return nullptr;
        }

        return sound_fx.get();
    }

    const std::string &SoundFX::GetName() const
    {
        return params.Name;
    }

    const std::string &SoundFX::GetDescription() const
    {
        return params.Description;
    }

    const std::string &SoundFX::GetFilePath() const
    {
        return params.FilePath;
    }

    double SoundFX::GetDuration() const
    {
        return params.Duration;
    }
}
