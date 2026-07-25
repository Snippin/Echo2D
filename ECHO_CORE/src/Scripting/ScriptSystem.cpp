#include "Core/Systems/ScriptSystem.h"

#include "Core/ECS/Components/ScriptComponent.h"
#include "Core/ECS/Entity.h"

#include <Logger/Logger.h>

namespace ECHO_CORE::SYSTEMS
{
    ScriptSystem::ScriptSystem(ECHO_CORE::ECS::Registry &registry) :
        registry{registry}, main_loaded{}
    {
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

            auto &script = ent.GetComponent<
                ECHO_CORE::ECS::ScriptComponent>();

            auto status = script.Update(entity);
            if (!status.valid())
            {
                sol::error error = status;
                ECHO_ERROR("Error running Update script: {}", error.what());
            }
        }
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

            auto &script = ent.GetComponent<
                ECHO_CORE::ECS::ScriptComponent>();

            auto status = script.Render(entity);
            if (!status.valid())
            {
                sol::error error = status;
                ECHO_ERROR("Error running Render script: {}", error.what());
            }
        }
    }
}
