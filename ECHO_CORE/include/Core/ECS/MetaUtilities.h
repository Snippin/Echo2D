#pragma once

#include <Logger/Logger.h>

#include <entt.hpp>
#include <sol/sol.hpp>

namespace ECHO_CORE::UTILS
{
    [[nodiscard]] entt::id_type GetIdType(const sol::table &component);

    template <typename ...Args>
    inline auto InvokeMetaFunction(entt::meta_type meta, entt::id_type func_id,
        Args&& ...args)
    {
        if (!meta)
        {
            ECHO_ERROR("No entt::meta_type has been given or is invalid");
            assert(false && "No entt::meta_type has been given or is invalid");
            return entt::meta_any{};
        }

        if (auto meta_func = meta.func(func_id); meta_func)
        {
            return meta_func.invoke({}, std::forward<Args>(args)...);
        }

        return entt::meta_any{};
    }

    template <typename ...Args>
    inline auto InvokeMetaFunction(entt::id_type id, entt::id_type func_id,
        Args&& ...args)
    {
        return InvokeMetaFunction(entt::resolve(id), func_id,
            std::forward<Args>(args)...);
    }
}
