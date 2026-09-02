#include "Core/Systems/ScriptSystem.h"

#include "Core/ECS/Components/AnimationComponent.h"
#include "Core/ECS/Components/BoxColliderComponent.h"
#include "Core/ECS/Components/CircleColliderComponent.h"
#include "Core/ECS/Components/ScriptComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/ECS/Entity.h"
#include "Core/Resources/AssetManager.h"
#include "Core/Scripting/GlmBindings.h"
#include "Core/Scripting/InputManager.h"
#include "Core/Scripting/RenderingBindings.h"
#include "Core/Scripting/SoundBindings.h"

#include <EchoUtilities/Timer.h>
#include <Logger/Logger.h>

namespace ECHO_CORE::SYSTEMS
{
    ScriptSystem::ScriptSystem(ECHO_CORE::ECS::Registry &registry) :
        registry{registry}, main_loaded{}
    {
    }

    void ScriptSystem::RegisterLuaBindings(sol::state &lua,
        ECHO_CORE::ECS::Registry &registry)
    {
        using namespace ECHO_CORE;

        ECHO_RESOURCES::AssetManager::CreateLuaBind(lua, registry);

        SCRIPTING::GLMBindings::CreateLuaBind(lua);
        InputManager::CreateLuaBind(lua);
        SCRIPTING::SoundBindings::CreateLuaBind(lua, registry);
        SCRIPTING::RenderingBindings::CreateLuaBind(lua, registry);

        ECHO_UTIL::Timer::CreateLuaBind(lua);

        ECS::Registry::CreateLuaBind(lua, registry);
        ECS::Entity::CreateLuaBind(lua, registry);
        ECS::TransformComponent::CreateLuaBind(lua);
        ECS::SpriteComponent::CreateLuaBind(lua, registry);
        ECS::AnimationComponent::CreateLuaBind(lua);
        ECS::BoxColliderComponent::CreateLuaBind(lua);
        ECS::CircleColliderComponent::CreateLuaBind(lua);

        ECS::Entity::RegisterMetaComponent<ECS::TransformComponent>();
        ECS::Entity::RegisterMetaComponent<ECS::SpriteComponent>();
        ECS::Entity::RegisterMetaComponent<ECS::AnimationComponent>();
        ECS::Entity::RegisterMetaComponent<ECS::BoxColliderComponent>();
        ECS::Entity::RegisterMetaComponent<ECS::CircleColliderComponent>();

        ECS::Registry::RegisterMetaComponent<ECS::TransformComponent>();
        ECS::Registry::RegisterMetaComponent<ECS::TransformComponent>();
        ECS::Registry::RegisterMetaComponent<ECS::AnimationComponent>();
        ECS::Registry::RegisterMetaComponent<ECS::BoxColliderComponent>();
        ECS::Registry::RegisterMetaComponent<ECS::CircleColliderComponent>();
    }

    void ScriptSystem::RegisterLuaFunctions(sol::state &lua)
    {
        lua.set_function(
            "run_script",
            [&lua](const std::string &path)
            {
                try
                {
                    lua.safe_script_file(path);
                }
                catch (const sol::error &error)
                {
                    ECHO_ERROR("Failed to load lua script: {}", error.what());
                    return false;
                }

                return true;
            }
        );
    }

    bool ScriptSystem::LoadMainScript(sol::state &lua)
    {
        try
        {
            auto result = lua.safe_script_file("./assets/scripts/main.lua");
        }
        catch (const sol::error &err)
        {
            auto str = err.what();
            ECHO_ERROR("Error loading main lua script: {}", str);
            return false;
        }

        sol::table main_lua = lua["main"];

        sol::optional<sol::table> update_exists = main_lua[1];
        if (update_exists == sol::nullopt)
        {
            ECHO_ERROR("There is no `Update` function in main.lua");
            return false;
        }

        sol::table update_script = main_lua[1];
        sol::function update_function = update_script["Update"];

        sol::optional<sol::table> render_exists = main_lua[2];
        if (render_exists == sol::nullopt)
        {
            ECHO_ERROR("There is no `Render` function in main.lua");
            return false;
        }

        sol::table render_script = main_lua[2];
        sol::function render_function = render_script["Render"];

        ECHO_CORE::ECS::Entity main_lua_script{registry, "main_script", ""};
        main_lua_script.AddComponent<ECHO_CORE::ECS::ScriptComponent>(
            ECHO_CORE::ECS::ScriptComponent{
                .Update = update_function,
                .Render = render_function,
            });

        main_loaded = true;
        return true;
    }

    void ScriptSystem::Update()
    {
        if (!main_loaded)
        {
            ECHO_ERROR("Main lua script has not been loaded");
            return;
        }

        auto view = registry.Get().view<ECHO_CORE::ECS::ScriptComponent>();

        for (const auto &entity : view)
        {
            ECHO_CORE::ECS::Entity ent{registry, entity};
            if (ent.GetName() != "main_script")
            {
                continue;
            }

            const auto &script = ent.GetComponent<
                ECHO_CORE::ECS::ScriptComponent>();

            auto status = script.Update(entity);
            if (!status.valid())
            {
                sol::error error = status;
                ECHO_ERROR("Error running Update script: {}", error.what());
            }
        }

        const auto &lua = registry.GetContext<std::shared_ptr<sol::state>>();
        lua->collect_garbage();
    }

    void ScriptSystem::Render()
    {
        if (!main_loaded)
        {
            ECHO_ERROR("Main lua script has not been loaded");
            return;
        }

        auto view = registry.Get().view<ECHO_CORE::ECS::ScriptComponent>();

        for (const auto &entity : view)
        {
            ECHO_CORE::ECS::Entity ent{registry, entity};
            if (ent.GetName() != "main_script")
            {
                continue;
            }

            const auto &script = ent.GetComponent<
                ECHO_CORE::ECS::ScriptComponent>();

            auto status = script.Render(entity);
            if (!status.valid())
            {
                sol::error error = status;
                ECHO_ERROR("Error running Render script: {}", error.what());
            }
        }

        const auto &lua = registry.GetContext<std::shared_ptr<sol::state>>();
        lua->collect_garbage();
    }
}
