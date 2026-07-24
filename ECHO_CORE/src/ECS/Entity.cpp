#include "Core/ECS/Entity.h"

#include "Core/Ecs/Components/Identity.h"

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
