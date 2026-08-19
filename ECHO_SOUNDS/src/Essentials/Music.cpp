#include "Sounds/Essentials/Music.h"

namespace ECHO_SOUNDS
{
    Music::Music(const SoundParams &params, MusicPtr music) :
        params{params}, music{std::move(music)}
    {
    }

    Mix_Music *Music::GetMusicPtr() const
    {
        if (!music)
        {
            return nullptr;
        }

        return music.get();
    }

    const std::string &Music::GetName() const
    {
        return params.Name;
    }

    const std::string &Music::GetDescription() const
    {
        return params.Description;
    }

    const std::string &Music::GetFilePath() const
    {
        return params.FilePath;
    }

    double Music::GetDuration() const
    {
        return params.Duration;
    }
}
