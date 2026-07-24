#pragma once

#include "Entity.h"

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
}
