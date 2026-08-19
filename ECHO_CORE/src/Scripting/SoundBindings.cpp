#include "Core/Scripting/SoundBindings.h"

#include "Core/ECS/Registry.h"
#include "Core/Resources/AssetManager.h"

#include <Logger/Logger.h>
#include <Sounds/Audio/MusicPlayer.h>

namespace ECHO_CORE::SCRIPTING
{
    void SoundBindings::CreateLuaBind(sol::state &lua,
        ECHO_CORE::ECS::Registry &registry)
    {
        const auto &music_player =
            registry.GetContext<std::shared_ptr<ECHO_SOUNDS::MusicPlayer>>();
        if (!music_player)
        {
            ECHO_ERROR("Failed to bind `MusicPlayer` to lua -- "
                "Not in registry");
            return;
        }

        const auto &asset_manager = registry.GetContext<
            std::shared_ptr<ECHO_RESOURCES::AssetManager>>();
        if (!asset_manager)
        {
            ECHO_ERROR("Failed to bind `MusicPlayer` to lua -- "
                "`AssetManager` not in registry");
            return;
        }

        lua.new_usertype<ECHO_SOUNDS::MusicPlayer>(
            "Music",
            sol::no_constructor,
            "Play", sol::overload(
                [&asset_manager, &music_player](const std::string &name,
                    int loops)
                {
                    auto music = asset_manager->GetMusic(name);
                    if (!music)
                    {
                        ECHO_ERROR("Failed to get music [{}] from "
                            "`AssetManager`", name);
                        return;
                    }

                    music_player->Play(*music, loops);
                },
                [&asset_manager, &music_player](const std::string &name)
                {
                    auto music = asset_manager->GetMusic(name);
                    if (!music)
                    {
                        ECHO_ERROR("Failed to get music [{}] from "
                            "`AssetManager`", name);
                        return;
                    }

                    music_player->Play(*music, -1);
                }
            ),
            "Stop",
            [&music_player]()
            {
                music_player->Stop();
            },
            "Pause",
            [&music_player]()
            {
                music_player->Pause();
            },
            "Resume",
            [&music_player]()
            {
                music_player->Resume();
            },
            "SetVolume",
            [&music_player](int volume)
            {
                music_player->SetVolume(volume);
            },
            "IsPlaying",
            [&music_player]()
            {
                return music_player->IsPlaying();
            }
        );
    }
}
