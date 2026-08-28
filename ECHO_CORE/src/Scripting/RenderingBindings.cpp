#include "Core/Scripting/RenderingBindings.h"

#include "Core/ECS/Registry.h"

#include <Logger/Logger.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/Renderer.h>
#include <Rendering/Essentials/Primitives.h>

namespace ECHO_CORE::SCRIPTING
{
    void RenderingBindings::CreateLuaBind(sol::state &lua,
        ECHO_CORE::ECS::Registry &registry)
    {
        // Bind primitives
        lua.new_usertype<ECHO_RENDERING::Line>(
            "Line",
            sol::call_constructor,
            sol::factories(
                [](const glm::vec2 &p1, const glm::vec2 &p2,
                    const ECHO_RENDERING::Color &color)
                {
                    return ECHO_RENDERING::Line{
                        .P1 = p1,
                        .P2 = p2,
                        .Color = color
                    };
                }
            ),
            "P1", &ECHO_RENDERING::Line::P1,
            "P2", &ECHO_RENDERING::Line::P2,
            "Color", &ECHO_RENDERING::Line::Color
        );

        lua.new_usertype<ECHO_RENDERING::Rect>(
            "Rect",
            sol::call_constructor,
            sol::factories(
                [](const glm::vec2 &position, float width, float height,
                    const ECHO_RENDERING::Color &color)
                {
                    return ECHO_RENDERING::Rect{
                        .Position = position,
                        .Width = width,
                        .Height = height,
                        .Color = color
                    };
                }
            ),
            "Position", &ECHO_RENDERING::Rect::Position,
            "Width", &ECHO_RENDERING::Rect::Width,
            "Height", &ECHO_RENDERING::Rect::Height,
            "Color", &ECHO_RENDERING::Rect::Color
        );

        lua.new_usertype<ECHO_RENDERING::Circle>(
            "Circle",
            sol::call_constructor,
            sol::factories(
                [](const glm::vec2 &position, float radius, float thickness,
                    const ECHO_RENDERING::Color &color)
                {
                    return ECHO_RENDERING::Circle{
                        .Position = position,
                        .Radius = radius,
                        .Thickness = thickness,
                        .Color = color
                    };
                },
                [](const glm::vec2 &position, float radius,
                    const ECHO_RENDERING::Color &color)
                {
                    return ECHO_RENDERING::Circle{
                        .Position = position,
                        .Radius = radius,
                        .Thickness = 1.f,
                        .Color = color
                    };
                }
            ),
            "Position", &ECHO_RENDERING::Circle::Position,
            "Radius", &ECHO_RENDERING::Circle::Radius,
            "Thickness", &ECHO_RENDERING::Circle::Thickness,
            "Color", &ECHO_RENDERING::Circle::Color
        );

        // Bind renderer
        const auto &renderer =
            registry.GetContext<std::shared_ptr<ECHO_RENDERING::Renderer>>();
        if (!renderer)
        {
            ECHO_ERROR("Failed to bind `Renderer` to lua -- Not in registry");
            return;
        }

        lua.set_function(
            "DrawLine",
            sol::overload(
                [&renderer](const ECHO_RENDERING::Line &line)
                {
                    renderer->DrawLine(line);
                },
                [&renderer](const glm::vec2 &p1, const glm::vec2 &p2,
                    const ECHO_RENDERING::Color &color)
                {
                    renderer->DrawLine(p1, p2, color);
                }
            )
        );

        lua.set_function(
            "DrawRect", sol::overload(
                [&renderer](const ECHO_RENDERING::Rect &rect)
                {
                    renderer->DrawRect(rect);
                },
                [&renderer](const glm::vec2 &position, float width,
                    float height, const ECHO_RENDERING::Color &color)
                {
                    renderer->DrawRect(position, width, height, color);
                }
            )
        );

        lua.set_function(
            "DrawFilledRect", sol::overload(
                [&renderer](const ECHO_RENDERING::Rect &rect)
                {
                    renderer->DrawFilledRect(rect);
                },
                [&renderer](const glm::vec2 &position, float width,
                    float height, const ECHO_RENDERING::Color &color)
                {
                    renderer->DrawFilledRect(ECHO_RENDERING::Rect{
                        .Position = position,
                        .Width = width,
                        .Height = height,
                        .Color = color
                    });
                }
            )
        );

        lua.set_function(
            "DrawCircle", sol::overload(
                [&renderer](const ECHO_RENDERING::Circle &circle)
                {
                    renderer->DrawCircle(circle);
                },
                [&renderer](const glm::vec2 &position, float radius,
                    const ECHO_RENDERING::Color &color, float thickness)
                {
                    renderer->DrawCircle(position, radius, color, thickness);
                },
                [&renderer](const glm::vec2 &position, float radius,
                    const ECHO_RENDERING::Color &color)
                {
                    renderer->DrawCircle(position, radius, color, 1.f);
                }
            )
        );

        // Bind camera
        const auto &camera =
            registry.GetContext<std::shared_ptr<ECHO_RENDERING::Camera2D>>();
        if (!camera)
        {
            ECHO_ERROR("Failed to bind `Camera2D` to lua -- Not in registry");
            return;
        }

        lua.new_usertype<ECHO_RENDERING::Camera2D>(
            "Camera",
            sol::no_constructor,
            "Get",
            [&camera]() { return *camera; },
            "Position",
            [&camera]() { return camera->GetPosition(); },
            "SetPosition",
            [&camera](const glm::vec2 &position)
            {
                camera->SetPosition(position);
            },
            "Scale",
            [&camera]() { return camera->GetScale(); },
            "SetScale",
            [&camera](float scale)
            {
                camera->SetScale(scale);
            }
        );
    }
}
