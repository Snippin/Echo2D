#pragma once

#include "Entity.h"
#include <sol/sol.hpp>

#include <entt.hpp>

namespace ECHO_CORE::ECS
{
    template<typename TComponent, typename ...Args>
    TComponent &Entity::AddComponent(Args && ...args)
    {
        auto &reg = registry.Get();
        return reg.emplace<TComponent>(entity, std::forward<Args>(args)...);
    }

    template<typename TComponent, typename ...Args>
    TComponent &Entity::ReplaceComponent(Args && ...args)
    {
        auto &reg = registry.Get();
        if (reg.all_of<TComponent>(entity))
        {
            return reg.replace<TComponent>(entity, std::forward<Args>(args)...);
        }
        else
        {
            return reg.emplace<TComponent>(entity, std::forward<Args>(args)...);
        }
    }

    template<typename TComponent>
    TComponent &Entity::GetComponent()
    {
        auto &reg = registry.Get();
        return reg.get<TComponent>(entity);
    }

    template<typename TComponent>
    bool Entity::HasComponent()
    {
        auto &reg = registry.Get();
        return reg.all_of<TComponent>(entity);
    }

    template<typename TComponent>
    void Entity::RemoveComponent()
    {
        auto &reg = registry.Get();
        reg.remove<TComponent>(entity);
    }

    template<typename TComponent>
    auto add_component(Entity &entity, const sol::table &comp, 
        sol::this_state s)
    {
        auto &component = entity.AddComponent<TComponent>(
            comp.valid() ? comp.as<TComponent>() : TComponent{}
        );

        return sol::make_reference(s, std::ref(component));
    }

    template<typename TComponent>
    inline void Entity::RegisterMetaComponent()
    {
        using namespace entt::literals;

        entt::meta_factory<TComponent>()
            .type(entt::type_hash<TComponent>::value())
            .template func<&add_component<TComponent>>("add_component"_hs);
    }
}
