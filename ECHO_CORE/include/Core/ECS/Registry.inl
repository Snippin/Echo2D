#include "Registry.h"

namespace ECHO_CORE::ECS
{
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

    template<typename TComponent>
    entt::runtime_view &add_component_to_view(Registry *registry,
        entt::runtime_view &view)
    {
        return view.iterate(registry->Get().storage<TComponent>());
    }

    template<typename TComponent>
    entt::runtime_view &exclude_component_to_view(Registry *registry, entt::runtime_view &view)
    {
        return view.exclude(registry->Get().storage<TComponent>());
    }

    template <typename TComponent>
    inline void Registry::RegisterMetaComponent()
    {
        using namespace entt::literals;

        entt::meta_factory<TComponent>()
            .type(entt::type_hash<TComponent>::value())
            .template func<&add_component_to_view<TComponent>>("add_component_to_view"_hs)
            .template func<&exclude_component_to_view<TComponent>>("exclude_component_to_view"_hs);
    }
}
