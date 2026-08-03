#include "Windowing/Inputs/Mouse.h"
#include "Windowing/Inputs/MouseButtons.h"

#include <Logger/Logger.h>

#include <SDL.h>

namespace ECHO_WINDOW::INPUTS
{
    Mouse::Mouse() :
        buttons{{MOUSE_LEFT, Button{}},
            {MOUSE_MIDDLE, Button{}},
            {MOUSE_RIGHT, Button{}}},
        x{0}, y{0}, wheel_x{0}, wheel_y{0}, mouse_moving{}
    {
    }

    void Mouse::Update()
    {
        for (auto &[btn, button] : buttons)
        {
            button.Reset();
        }

        wheel_x = 0;
        wheel_y = 0;
        mouse_moving = false;
    }

    void Mouse::OnBtnPressed(int btn)
    {
        if (btn == MOUSE_UNKNOWN)
        {
            ECHO_ERROR("Mouse btn [{}] is unknown", btn);
            return;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Mouse btn [{}] does not exist", btn);
            return;
        }

        itr->second.Update(true);
    }

    void Mouse::OnBtnReleased(int btn)
    {
        if (btn == MOUSE_UNKNOWN)
        {
            ECHO_ERROR("Mouse btn [{}] is unknown", btn);
            return;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Mouse btn [{}] does not exist", btn);
            return;
        }

        itr->second.Update(false);
    }

    bool Mouse::IsBtnPressed(int btn) const
    {
        if (btn == MOUSE_UNKNOWN)
        {
            ECHO_ERROR("Mouse btn [{}] is unknown", btn);
            return false;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Mouse btn [{}] does not exist", btn);
            return false;
        }

        return itr->second.IsPressed;
    }

    bool Mouse::IsBtnJustPressed(int btn) const
    {
        if (btn == MOUSE_UNKNOWN)
        {
            ECHO_ERROR("Mouse btn [{}] is unknown", btn);
            return false;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Mouse btn [{}] does not exist", btn);
            return false;
        }

        return itr->second.JustPressed;
    }

    bool Mouse::IsBtnJustReleased(int btn) const
    {
        if (btn == MOUSE_UNKNOWN)
        {
            ECHO_ERROR("Mouse btn [{}] is unknown", btn);
            return false;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Mouse btn [{}] does not exist", btn);
            return false;
        }

        return itr->second.JustReleased;
    }

    std::tuple<int, int> Mouse::GetMouseScreenPosition()
    {
        SDL_GetMouseState(&x, &y);
        return std::make_tuple(x, y);
    }

    int Mouse::GetMouseWheelX() const
    {
        return wheel_x;
    }

    void Mouse::SetMouseWheelX(int wheel)
    {
        wheel_x = wheel;
    }

    int Mouse::GetMouseWheelY() const
    {
        return wheel_y;
    }

    void Mouse::SetMouseWheelY(int wheel)
    {
        wheel_y = wheel;
    }

    bool Mouse::IsMouseMoving() const
    {
        return mouse_moving;
    }

    void Mouse::SetMouseMoving(bool moving)
    {
        mouse_moving = moving;
    }
}
