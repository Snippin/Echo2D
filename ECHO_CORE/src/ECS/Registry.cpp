#include "Core/ECS/Registry.h"

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
}
