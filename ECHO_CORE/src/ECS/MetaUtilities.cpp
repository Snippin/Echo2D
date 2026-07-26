#include "Core/ECS/MetaUtilities.h"

namespace ECHO_CORE::UTILS
{
    entt::id_type GetIdType(const sol::table &component)
    {
        if (!component.valid())
        {
            ECHO_ERROR("Failed to get type id -- Component has not been "
                "exposed to lua");
            assert(component.valid() && "Failed to get type id -- Component "
                "has not been exposed to lua");
            return -1;
        }

        const auto func = component["type_id"].get<sol::function>();
        assert(func.valid() &&
            "[type_id] - function has not been exposed to lua"
            "\nEnsure all components and types have `type_id` function when "
            "making new usertype"
        );

        return func.valid() ? func().get<entt::id_type>() : -1;
    }
}
