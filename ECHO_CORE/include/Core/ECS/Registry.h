#pragma once

#include <entt.hpp>

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
    private:
        std::unique_ptr<entt::registry> registry;
    };

    template<typename TContext>
    inline TContext Registry::AddContext(TContext context)
    {
        return registry->ctx().emplace<TContext>(context);
    }

    template<typename TContext>
    inline TContext &Registry::GetContext()
    {
        return registry->ctx().get<TContext>();
    }
}
