#include "Windowing/Inputs/Button.h"

namespace ECHO_WINDOW::INPUTS
{
    void Button::Update(bool is_pressed)
    {
        JustPressed = !IsPressed && is_pressed;
        JustReleased = IsPressed && !is_pressed;
        IsPressed = is_pressed;
    }

    void Button::Reset()
    {
        JustPressed = false;
        JustReleased = false;
    }
}
