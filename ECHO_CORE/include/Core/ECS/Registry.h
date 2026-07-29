#pragma once

#include <entt.hpp>
#include <sol/sol.hpp>

namespace ECHO_CORE::ECS
{
    class Registry
    {
    public:
        Registry();
        ~Registry() = default;

        entt::registry &Get();
        entt::entity CreateEntity();

        template<typename TContext>
        TContext AddContext(TContext context);
        template<typename TContext>
        TContext &GetContext();

        static void CreateLuaBind(sol::state &lua, Registry &registry);

        template<typename TComponent>
        static void RegisterMetaComponent();

    private:
        std::unique_ptr<entt::registry> registry;
    };

    template<typename TComponent>
    entt::runtime_view &add_component_to_view(Registry *registry, 
        entt::runtime_view &view);

    template<typename TComponent>
    entt::runtime_view &exclude_component_to_view(Registry *registry,
        entt::runtime_view &view);
}

#include "Registry.inl"
