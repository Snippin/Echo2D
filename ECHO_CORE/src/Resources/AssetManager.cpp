#include "Core/Resources/AssetManager.h"

#include <Logger/Logger.h>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>

namespace ECHO_RESOURCES
{
    void AssetManager::CreateLuaBind(sol::state &lua,
        ECHO_CORE::ECS::Registry &registry)
    {
        const auto &asset_manager =
            registry.GetContext<std::shared_ptr<AssetManager>>();
        if (!asset_manager)
        {
            ECHO_ERROR("Failed to bind `AssetManager` to lua - Does not exist "
                "in registry");
            return;
        }

        lua.new_usertype<AssetManager>(
            "AssetManager",
            sol::no_constructor,
            "AddTexture",
            [&asset_manager](const std::string &name, const std::string &path,
                bool pixel_art)
            {
                return asset_manager->AddTexture(name, path, pixel_art);
            },
            "AddMusic",
            [&asset_manager](const std::string &name, const std::string &path)
            {
                return asset_manager->AddMusic(name, path);
            },
            "AddSoundFX",
            [&asset_manager](const std::string &name, const std::string &path)
            {
                return asset_manager->AddSoundFX(name, path);
            }
        );
    }

    bool AssetManager::AddTexture(const std::string &name,
        const std::string &path, bool pixel_art)
    {
        // Check if texture exists
        if (textures.contains(name))
        {
            ECHO_ERROR("Failed to add texture [{}] -- Already exists", name);
            return false;
        }

        // Create texture
        auto texture = std::move(ECHO_RENDERING::TextureLoader::Create(
            pixel_art ? ECHO_RENDERING::Texture::TextureType::PIXEL :
            ECHO_RENDERING::Texture::TextureType::BLENDED, path));

        if (!texture)
        {
            ECHO_ERROR("Failed to load texture [{}] at [{}]", name, path);
            return false;
        }

        textures.emplace(name, std::move(texture));
        return true;
    }

    const ECHO_RENDERING::Texture &AssetManager::GetTexture(
        const std::string &name)
    {
        auto itr = textures.find(name);
        if (itr == textures.end())
        {
            ECHO_ERROR("Failed to get texture [{}] -- Does not exist", name);
            return ECHO_RENDERING::Texture();
        }

        return *itr->second;
    }

    bool AssetManager::AddShader(const std::string &name,
        const std::string &base_path)
    {
        // Check if shader exists
        if (shaders.contains(name))
        {
            ECHO_ERROR("Failed to add shader [{}] -- Already exists", name);
            return false;
        }

        // Create and load shader
        auto shader = std::move(ECHO_RENDERING::ShaderLoader::Create(
            base_path));

        if (!shader)
        {
            ECHO_ERROR("Failed to load shader [{}] at [{}]", name, base_path);
            return false;
        }

        shaders.emplace(name, std::move(shader));
        return true;
    }

    ECHO_RENDERING::Shader &AssetManager::GetShader(const std::string &name)
    {
        auto itr = shaders.find(name);
        if (itr == shaders.end())
        {
            ECHO_ERROR("Failed to get shader [{}] -- Does not exist", name);
            ECHO_RENDERING::Shader shader{};
            return shader;
        }

        return *itr->second;
    }

    bool AssetManager::AddMusic(const std::string &name,
        const std::string &path)
    {
        // Check if music exists
        if (musics.contains(name))
        {
            ECHO_ERROR("Failed to add music [{}] -- Already exists", name);
            return false;
        }

        // Create music
        Mix_Music *music = Mix_LoadMUS(path.c_str());

        if (!music)
        {
            ECHO_ERROR("Failed to load music [{}] at [{}] -- Mixer Error: {}",
                name, path, Mix_GetError());
            return false;
        }

        // Create params
        ECHO_SOUNDS::SoundParams params{
            .Name = name,
            .FilePath = path,
            .Duration = Mix_MusicDuration(music)
        };

        auto music_ptr =
            std::make_shared<ECHO_SOUNDS::Music>(params, MusicPtr{music});

        if (!music_ptr)
        {
            ECHO_ERROR("Failed to create music ptr for [{}]", name);
            return false;
        }

        musics.emplace(name, std::move(music_ptr));
        return true;
    }

    std::shared_ptr<ECHO_SOUNDS::Music> AssetManager::GetMusic(
        const std::string &name)
    {
        auto itr = musics.find(name);
        if (itr == musics.end())
        {
            ECHO_ERROR("Failed to get music [{}] -- Does not exist", name);
            return nullptr;
        }

        return itr->second;
    }

    bool AssetManager::AddSoundFX(const std::string &name, const std::string &path)
    {
        // Check if sound fx exists
        if (sound_fxs.contains(name))
        {
            ECHO_ERROR("Failed to add sound fx [{}] -- Already exists", name);
            return false;
        }

        // Create sound fx
        Mix_Chunk *sound_fx = Mix_LoadWAV(path.c_str());

        if (!sound_fx)
        {
            ECHO_ERROR("Failed to load sound fx [{}] at [{}] -- "
                "Mixer Error: {}", name, path, Mix_GetError());
            return false;
        }

        // Create params
        ECHO_SOUNDS::SoundParams params{
            .Name = name,
            .FilePath = path,
            .Duration = sound_fx->alen / 176.4
        };

        auto sound_fx_ptr =
            std::make_shared<ECHO_SOUNDS::SoundFX>(params, SoundFXPtr{sound_fx});

        if (!sound_fx_ptr)
        {
            ECHO_ERROR("Failed to create sound fx ptr for [{}]", name);
            return false;
        }

        sound_fxs.emplace(name, std::move(sound_fx_ptr));
        return true;
    }

    std::shared_ptr<ECHO_SOUNDS::SoundFX> AssetManager::GetSoundFX(const std::string &name)
    {
        auto itr = sound_fxs.find(name);
        if (itr == sound_fxs.end())
        {
            ECHO_ERROR("Failed to get sound fx [{}] -- Does not exist", name);
            return nullptr;
        }

        return itr->second;
    }
}
