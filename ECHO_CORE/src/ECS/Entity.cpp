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
                [&](const std::string &name, const std::string &group)
                {
                    return Entity{registry, name, group};
                }
            ),
            "addComponent",
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
