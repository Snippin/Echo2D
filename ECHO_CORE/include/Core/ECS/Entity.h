#pragma once

#include "Registry.h"

namespace ECHO_CORE::ECS
{
    class Entity
    {
    public:
        Entity(Registry &registry);
        Entity(Registry &registry, const std::string &name = "",
            const std::string &group = "");
        ~Entity() = default;

        std::uint32_t Kill();

        entt::entity &GetEntity();
        entt::registry &GetRegistry();

        template <typename TComponent, typename ...Args>
        TComponent &AddComponent(Args &&...args);
        template <typename TComponent, typename ...Args>
        TComponent &ReplaceComponent(Args &&...args);
        template <typename TComponent>
        TComponent &GetComponent();
        template <typename TComponent>
        bool HasComponent();
        template <typename TComponent>
        void RemoveComponent();

    private:
        Registry &registry;
        entt::entity entity;

        std::string name;
        std::string group;
    };
}

#include "Entity.inl"
