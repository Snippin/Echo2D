#pragma once

#include "Registry.h"

#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    class Entity
    {
    public:
        Entity(Registry &registry);
        Entity(Registry &registry, const std::string &name = "",
            const std::string &group = "");
        Entity(Registry &registry, const entt::entity &entity);
        ~Entity() = default;

        static void CreateLuaBind(sol::state &lua, Registry &registry);

        template <typename TComponent>
        static void RegisterMetaComponent();

        const std::string &GetName() const;
        const std::string &GetGroup() const;

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
        auto RemoveComponent();

    private:
        Registry &registry;
        entt::entity entity;

        std::string name;
        std::string group;
    };

    template <typename TComponent>
    auto add_component(Entity &entity, const sol::table &component,
        sol::this_state s);
    template <typename TComponent>
    bool has_component(Entity &entity);
    template <typename TComponent>
    auto get_component(Entity &entity, sol::this_state s);
    template <typename TComponent>
    auto remove_component(Entity &entity);
}

#include "Entity.inl"
