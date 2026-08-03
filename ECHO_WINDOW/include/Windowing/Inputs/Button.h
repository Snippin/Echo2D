#pragma once

namespace ECHO_WINDOW::INPUTS
{
    struct Button
    {
        bool IsPressed{};
        bool JustPressed{};
        bool JustReleased{};

        void Update(bool is_pressed);
        void Reset();
    };
}
