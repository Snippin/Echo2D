#pragma once

#include "../ECS/Registry.h"

#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <Sounds/Essentials/Music.h>

#include <sol/sol.hpp>

#include <map>
#include <memory>
#include <string>

namespace ECHO_RESOURCES
{
    class AssetManager
    {
    public:
        AssetManager() = default;
        ~AssetManager() = default;

        static void CreateLuaBind(sol::state &lua,
            ECHO_CORE::ECS::Registry &registry);

        bool AddTexture(const std::string &name,
            const std::string &path, bool pixel_art = true);
        const ECHO_RENDERING::Texture &GetTexture(const std::string &name);

        bool AddShader(const std::string &name, const std::string &base_path);
        ECHO_RENDERING::Shader &GetShader(const std::string &name);

        bool AddMusic(const std::string &name, const std::string &path);
        std::shared_ptr<ECHO_SOUNDS::Music> GetMusic(const std::string &name);

    private:
        std::map<std::string, std::shared_ptr<ECHO_RENDERING::Texture>>
            textures;
        std::map<std::string, std::shared_ptr<ECHO_RENDERING::Shader>> shaders;
        std::map<std::string, std::shared_ptr<ECHO_SOUNDS::Music>> musics;
    };
}
