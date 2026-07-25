#pragma once

#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>

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

        bool AddTexture(const std::string &name,
            const std::string &path, bool pixel_art = true);
        const ECHO_RENDERING::Texture &GetTexture(const std::string &name);

        bool AddShader(const std::string &name, const std::string &base_path);
        const ECHO_RENDERING::Shader &GetShader(const std::string &name);

    private:
        std::map<std::string, std::shared_ptr<ECHO_RENDERING::Texture>>
            textures;
        std::map<std::string, std::shared_ptr<ECHO_RENDERING::Shader>> shaders;
    };
}
