#pragma once

#include "Button.h"

#include <EchoUtilities/SDLWrapper.h>

#include <map>
#include <string>

namespace ECHO_WINDOW::INPUTS
{
    class Gamepad
    {
    public:
        Gamepad(ControllerPtr controller_ptr);
        ~Gamepad() = default;

        void Update();

        void OnBtnPressed(int btn);
        void OnBtnReleased(int btn);

        bool IsBtnPressed(int btn) const;
        bool IsBtnJustPressed(int btn) const;
        bool IsBtnJustReleased(int btn) const;

        bool IsGamepadPresent() const;

        Sint16 GetAxisPosition(Uint8 axis) const;
        void SetAxisPositionValue(Uint8 axis, Sint16 value);

        Uint8 GetJoystickHatValue() const;
        void SetJoystickHatValue(Uint8 value);

        bool CheckJoystickID(SDL_JoystickID id) const;
        const std::string &GetName() const;

    private:
        ControllerPtr controller;
        std::map<int, Button> buttons;

        SDL_JoystickID joystick_id;
        std::map<Uint8, Sint16> axis_values;
        Uint8 joystick_hat_value;

        std::string name;
    };
}
