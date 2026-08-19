#pragma once

namespace ECHO_SOUNDS
{
    class MusicPlayer
    {
    public:
        MusicPlayer();
        ~MusicPlayer();

        void Play(class Music &music, int loops = 0);
        void Stop();
        void Pause();
        void Resume();
        void SetVolume(int volume);
        bool IsPlaying() const;
    };
}
