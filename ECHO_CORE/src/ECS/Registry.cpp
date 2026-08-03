#include "Core/ECS/Registry.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/MetaUtilities.h"

namespace ECHO_CORE::ECS
{
    Registry::Registry() :
        registry{nullptr}
    {
        registry = std::make_unique<entt::registry>();
    }

    entt::registry &Registry::Get()
    {
        return *registry;
    }

    entt::entity Registry::CreateEntity()
    {
        return registry->create();
    }

    void Registry::CreateLuaBind(sol::state &lua, Registry &registry)
    {
        using namespace entt::literals;

        lua.new_usertype<entt::runtime_view>(
            "RuntimeView",
            sol::no_constructor,
            "ForEach",
            [&registry](const entt::runtime_view &view,
                const sol::function &callback, sol::this_state s)
            {
                if (!callback.valid())
                {
                    return;
                }

                for (auto entity : view)
                {
                    Entity new_ent{registry, entity};
                    callback(new_ent);
                }
            },
            "Exclude",
            [&registry](entt::runtime_view &view, const sol::variadic_args &va)
            {
                for (const auto &type : va)
                {
                    if (!type.as<sol::table>().valid())
                    {
                        continue;
                    }

                    const auto excluded_view =
                        ECHO_CORE::UTILS::InvokeMetaFunction(
                            ECHO_CORE::UTILS::GetIdType(type),
                            "exclude_component_to_view"_hs,
                            &registry, view
                        );

                    view = excluded_view ?
                        excluded_view.cast<entt::runtime_view>() : view;
                }
            }
        );

        lua.new_usertype<Registry>(
            "Registry",
            sol::no_constructor,
            "GetEntities",
            [&registry](const sol::variadic_args &va)
            {
                entt::runtime_view view{};

                for (const auto &type : va)
                {
                    if (!type.as<sol::table>().valid())
                    {
                        continue;
                    }

                    const auto entities = ECHO_CORE::UTILS::InvokeMetaFunction(
                        ECHO_CORE::UTILS::GetIdType(type),
                        "add_component_to_view"_hs,
                        &registry, view
                    );

                    view = entities ?
                        entities.cast<entt::runtime_view>() : view;
                }

                return view;
            },
            "clear",
            [&registry]() { registry.Get().clear(); }
        );
    }
}
