#include "Sounds/Audio/SoundFXPlayer.h"

#include "Sounds/Essentials/SoundFX.h"

#include <Logger/Logger.h>

namespace ECHO_SOUNDS
{
    void SoundFXPlayer::Play(const SoundFX &sound_fx) const
    {
        Play(sound_fx, 0, -1);
    }

    void SoundFXPlayer::Play(const SoundFX &sound_fx, int loops, int channel)
        const
    {
        if (!sound_fx.GetSoundFXPtr())
        {
            ECHO_ERROR("Failed to play sound fx [{}] on channel [{}] -- sound "
                "fx ptr is null", sound_fx.GetName(), channel);
            return;
        }

        if (Mix_PlayChannel(channel, sound_fx.GetSoundFXPtr(), loops) == -1)
        {
            ECHO_ERROR("Failed to play sound fx [{}] on channel [{}] -- "
                "ERROR: {}", sound_fx.GetName(), channel, Mix_GetError());
        }
    }

    void SoundFXPlayer::Stop(int channel) const
    {
        if (Mix_HaltChannel(channel) == -1)
        {
            ECHO_ERROR("Failed to halt sound fx for channel [{}]",
                channel == -1 ? "all" : std::to_string(channel));
        }
    }

    void SoundFXPlayer::SetVolume(int channel, int volume) const
    {
        if (volume < 0 || volume > 100)
        {
            ECHO_ERROR("Failed to set volume to [{}]. Must be between 0 - 100",
                volume);
            return;
        }

        // Scale volume (0% - 100%)
        int scaled_volume = static_cast<int>((volume / 100.f) * 128);
        Mix_Volume(channel, scaled_volume);
    }

    bool SoundFXPlayer::IsPlaying(int channel) const
    {
        return Mix_Playing(channel);
    }
}
