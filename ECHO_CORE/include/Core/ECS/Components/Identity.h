#pragma once

#include <string>

namespace ECHO_CORE::ECS
{
    struct Identity
    {
        std::string Name{"GameObject"};
        std::string Group{""};
        int32_t Entity_Id{-1};
    };
}
