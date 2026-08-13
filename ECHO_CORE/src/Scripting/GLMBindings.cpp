#define GLM_ENABLE_EXPERIMENTAL

#include "Core/Scripting/GLMBindings.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace ECHO_CORE::SCRIPTING
{
    // glm::vec2
    static void CreateVec2Bind(sol::state &lua)
    {
        // Multiply overloads
        auto multiply_overloads = sol::overload(
            [](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 * v2; },
            [](const glm::vec2 &v1, float value) { return v1 * value; },
            [](float value, const glm::vec2 &v1) { return v1 * value; }
        );

        // Divide overloads
        auto divide_overloads = sol::overload(
            [](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 / v2; },
            [](const glm::vec2 &v1, float value) { return v1 / value; },
            [](float value, const glm::vec2 &v1) { return v1 / value; }
        );

        // Addition overloads
        auto addition_overloads = sol::overload(
            [](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 + v2; },
            [](const glm::vec2 &v1, float value) { return v1 + value; },
            [](float value, const glm::vec2 &v1) { return v1 + value; }
        );

        // Subtraction overloads
        auto subtraction_overloads = sol::overload(
            [](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 - v2; },
            [](const glm::vec2 &v1, float value) { return v1 - value; },
            [](float value, const glm::vec2 &v1) { return v1 - value; }
        );

        lua.new_usertype<glm::vec2>(
            "Vec2",
            sol::call_constructor,
            sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y,
            sol::meta_function::multiplication, multiply_overloads,
            sol::meta_function::division, divide_overloads,
            sol::meta_function::addition, addition_overloads,
            sol::meta_function::subtraction, subtraction_overloads,
            "Length", [](const glm::vec2 &v) { return glm::length(v); },
            "LengthSqr", [](const glm::vec2 &v) { return glm::length2(v); }
        );
    }

    // glm::vec3
    static void CreateVec3Bind(sol::state &lua)
    {
        // Multiply overloads
        auto multiply_overloads = sol::overload(
            [](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 * v2; },
            [](const glm::vec3 &v1, float value) { return v1 * value; },
            [](float value, const glm::vec3 &v1) { return v1 * value; }
        );

        // Divide overloads
        auto divide_overloads = sol::overload(
            [](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 / v2; },
            [](const glm::vec3 &v1, float value) { return v1 / value; },
            [](float value, const glm::vec3 &v1) { return v1 / value; }
        );

        // Addition overloads
        auto addition_overloads = sol::overload(
            [](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 + v2; },
            [](const glm::vec3 &v1, float value) { return v1 + value; },
            [](float value, const glm::vec3 &v1) { return v1 + value; }
        );

        // Subtraction overloads
        auto subtraction_overloads = sol::overload(
            [](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 - v2; },
            [](const glm::vec3 &v1, float value) { return v1 - value; },
            [](float value, const glm::vec3 &v1) { return v1 - value; }
        );

        lua.new_usertype<glm::vec3>(
            "Vec3",
            sol::call_constructor,
            sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z,
            sol::meta_function::multiplication, multiply_overloads,
            sol::meta_function::division, divide_overloads,
            sol::meta_function::addition, addition_overloads,
            sol::meta_function::subtraction, subtraction_overloads,
            "Length", [](const glm::vec2 &v) { return glm::length(v); },
            "LengthSqr", [](const glm::vec2 &v) { return glm::length2(v); }
        );
    }

    // glm::vec4
    static void CreateVec4Bind(sol::state &lua)
    {
        // Multiply overloads
        auto multiply_overloads = sol::overload(
            [](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 * v2; },
            [](const glm::vec4 &v1, float value) { return v1 * value; },
            [](float value, const glm::vec4 &v1) { return v1 * value; }
        );

        // Divide overloads
        auto divide_overloads = sol::overload(
            [](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 / v2; },
            [](const glm::vec4 &v1, float value) { return v1 / value; },
            [](float value, const glm::vec4 &v1) { return v1 / value; }
        );

        // Addition overloads
        auto addition_overloads = sol::overload(
            [](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 + v2; },
            [](const glm::vec4 &v1, float value) { return v1 + value; },
            [](float value, const glm::vec4 &v1) { return v1 + value; }
        );

        // Subtraction overloads
        auto subtraction_overloads = sol::overload(
            [](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 - v2; },
            [](const glm::vec4 &v1, float value) { return v1 - value; },
            [](float value, const glm::vec4 &v1) { return v1 - value; }
        );

        lua.new_usertype<glm::vec4>(
            "Vec4",
            sol::call_constructor,
            sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
            "x", &glm::vec4::x,
            "y", &glm::vec4::y,
            "z", &glm::vec4::z,
            "w", &glm::vec4::w,
            sol::meta_function::multiplication, multiply_overloads,
            sol::meta_function::division, divide_overloads,
            sol::meta_function::addition, addition_overloads,
            sol::meta_function::subtraction, subtraction_overloads,
            "Length", [](const glm::vec2 &v) { return glm::length(v); },
            "LengthSqr", [](const glm::vec2 &v) { return glm::length2(v); }
        );
    }

    // Helper math functions
    static void MathFunctions(sol::state &lua)
    {
        lua.set_function(
            "Distance",
            sol::overload
            (
                [](const glm::vec2 &a, const glm::vec2 &b) { return glm::distance(a, b); },
                [](const glm::vec3 &a, const glm::vec3 &b) { return glm::distance(a, b); },
                [](const glm::vec4 &a, const glm::vec4 &b) { return glm::distance(a, b); }
            )
        );

        lua.set_function(
            "Lerp",
            [](float a, float b, float t) { return std::lerp(a, b, t); }
        );

        lua.set_function(
            "Clamp",
            sol::overload
            (
                [](int value, int min, int max) { return std::clamp(value, min, max); },
                [](float value, float min, float max) { return std::clamp(value, min, max); },
                [](double value, double min, double max) { return std::clamp(value, min, max); }
            )
        );
    }

    void GLMBindings::CreateLuaBind(sol::state &lua)
    {
        CreateVec2Bind(lua);
        CreateVec3Bind(lua);
        CreateVec4Bind(lua);

        MathFunctions(lua);
    }
}
