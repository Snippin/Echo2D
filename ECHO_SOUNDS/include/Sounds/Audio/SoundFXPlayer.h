#pragma once

namespace ECHO_SOUNDS
{
    class SoundFX;

    class SoundFXPlayer
    {
    public:
        SoundFXPlayer() = default;
        ~SoundFXPlayer() = default;

        void Play(const SoundFX &sound_fx) const;
        void Play(const SoundFX &sound_fx, int loops, int channel) const;
        void Stop(int channel) const;
        void SetVolume(int channel, int volume) const;
        bool IsPlaying(int channel) const;
    };
}
