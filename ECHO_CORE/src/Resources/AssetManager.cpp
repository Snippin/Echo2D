#include "Core/Resources/AssetManager.h"

#include <Logger/Logger.h>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>

namespace ECHO_RESOURCES
{
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
}
