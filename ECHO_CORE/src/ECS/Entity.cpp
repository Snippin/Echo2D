#include "Core/ECS/Entity.h"

#include "Core/Ecs/Components/Identity.h"
#include "Core/Ecs/MetaUtilities.h"

namespace ECHO_CORE::ECS
{
    Entity::Entity(Registry &registry) :
        Entity(registry, "GameObject", "")
    {
    }

    Entity::Entity(Registry &registry, const std::string &name,
        const std::string &group) :
        registry{registry}, entity{registry.CreateEntity()}, name{name},
        group{group}
    {
        AddComponent<Identity>(Identity{
            .Name = name,
            .Group = group,
            .Entity_Id = static_cast<int32_t>(entity)
            }
        );
    }

    Entity::Entity(Registry &registry, const entt::entity &entity) :
        registry{registry}, entity{entity}, name{""}, group{""}
    {
        if (HasComponent<Identity>())
        {
            const auto &id = GetComponent<Identity>();
            name = id.Name;
            group = id.Group;
        }
    }

    void Entity::CreateLuaBind(sol::state &lua, Registry &registry)
    {
        using namespace entt::literals;

        lua.new_usertype<Entity>(
            "Entity",
            sol::call_constructor,
            sol::factories(
                [&registry](const std::string &name, const std::string &group)
                {
                    return Entity{registry, name, group};
                },
                [&registry](std::int32_t id)
                {
                    return Entity{registry, static_cast<entt::entity>(id)};
                }
            ),
            "AddComponent",
            [](Entity &entity, const sol::table &comp, sol::this_state s) ->
                sol::object
            {
                // Check if valid lua/sol table
                if (!comp.valid())
                {
                    return sol::lua_nil_t{};
                }

                const auto component = ECHO_CORE::UTILS::InvokeMetaFunction(
                    ECHO_CORE::UTILS::GetIdType(comp),
                    "add_component"_hs,
                    entity, comp, s
                );

                return component ?
                    component.cast<sol::reference>() : sol::lua_nil_t{};
            },
            "HasComponent",
            [](Entity &entity, const sol::table &comp)
            {
                const auto has_comp = ECHO_CORE::UTILS::InvokeMetaFunction(
                    ECHO_CORE::UTILS::GetIdType(comp),
                    "has_component"_hs,
                    entity
                );

                return has_comp ? has_comp.cast<bool>() : false;
            },
            "GetComponent",
            [](Entity &entity, const sol::table &comp, sol::this_state s)
            {
                const auto component = ECHO_CORE::UTILS::InvokeMetaFunction(
                    ECHO_CORE::UTILS::GetIdType(comp),
                    "get_component"_hs,
                    entity, s
                );

                return component ?
                    component.cast<sol::reference>() : sol::lua_nil_t{};
            },
            "RemoveComponent",
            [](Entity &entity, const sol::table &comp)
            {
                const auto component = ECHO_CORE::UTILS::InvokeMetaFunction(
                    ECHO_CORE::UTILS::GetIdType(comp),
                    "remove_component"_hs,
                    entity
                );

                return component ?
                    component.cast<sol::reference>() : sol::lua_nil_t{};
            },
            "Name", &Entity::GetName,
            "Group", &Entity::GetGroup,
            "Kill", &Entity::Kill,
            "ID",
            [](Entity &entity)
            {
                return static_cast<int32_t>(entity.GetEntity());
            }
        );
    }

    const std::string &Entity::GetName() const
    {
        return name;
    }

    const std::string &Entity::GetGroup() const
    {
        return group;
    }

    std::uint32_t Entity::Kill()
    {
        return registry.Get().destroy(entity);
    }

    entt::entity &Entity::GetEntity()
    {
        return entity;
    }

    entt::registry &Entity::GetRegistry()
    {
        return registry.Get();
    }
}
