#pragma once

#include "Button.h"

#include <map>

namespace ECHO_WINDOW::INPUTS
{
    class Mouse
    {
    public:
        Mouse();
        ~Mouse() = default;

        void Update();
        void OnBtnPressed(int btn);
        void OnBtnReleased(int btn);

        bool IsBtnPressed(int btn) const;
        bool IsBtnJustPressed(int btn) const;
        bool IsBtnJustReleased(int btn) const;

        std::tuple<int, int> GetMouseScreenPosition();

        int GetMouseWheelX() const;
        void SetMouseWheelX(int wheel);

        int GetMouseWheelY() const;
        void SetMouseWheelY(int wheel);

        bool IsMouseMoving() const;
        void SetMouseMoving(bool moving);

    private:
        std::map<int, Button> buttons;

        int x;
        int y;
        int wheel_x;
        int wheel_y;
        bool mouse_moving;
    };
}
