#include "Sounds/Audio/MusicPlayer.h"

#include "Sounds/Essentials/Music.h"

#include <Logger/Logger.h>

namespace ECHO_SOUNDS
{
    MusicPlayer::MusicPlayer()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            ECHO_ERROR("Unable to open SDL Music Mixer - {}", Mix_GetError());
            return;
        }

        ECHO_LOG("CHANNELS ALLOCATED [{}]", Mix_AllocateChannels(16));
    }

    MusicPlayer::~MusicPlayer()
    {
        Mix_HaltMusic();
        Mix_Quit();
        ECHO_LOG("Music player closed");
    }

    void MusicPlayer::Play(Music &music, int loops)
    {
        if (!music.GetMusicPtr())
        {
            ECHO_ERROR("Failed to play music [{}] - `Mix_Music` is null",
                music.GetName());
            return;
        }

        if (Mix_PlayMusic(music.GetMusicPtr(), loops) == -1)
        {
            ECHO_ERROR("Failed to play music [{}] - Mix Error - {}",
                music.GetName(), Mix_GetError());
            return;
        }
    }

    void MusicPlayer::Stop()
    {
        Mix_HaltMusic();
    }

    void MusicPlayer::Pause()
    {
        Mix_PauseMusic();
    }

    void MusicPlayer::Resume()
    {
        Mix_ResumeMusic();
    }

    void MusicPlayer::SetVolume(int volume)
    {
        if (volume < 0 || volume > 100)
        {
            ECHO_ERROR("Failed to set volume to [{}]. Must be between 0 - 100",
                volume);
            return;
        }

        // Scale volume (0% - 100%)
        int scaled_volume = static_cast<int>((volume / 100.f) * 128);
        Mix_VolumeMusic(scaled_volume);
    }

    bool MusicPlayer::IsPlaying() const
    {
        return Mix_PlayingMusic();
    }
}
