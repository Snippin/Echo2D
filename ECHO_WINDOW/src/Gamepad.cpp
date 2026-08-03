#include "Windowing/Inputs/Gamepad.h"
#include "Windowing/Inputs/GamepadButtons.h"

#include <Logger/Logger.h>

namespace ECHO_WINDOW::INPUTS
{
    Gamepad::Gamepad(ControllerPtr controller_ptr) :
        controller{std::move(controller_ptr)},
        buttons{
            {GP_A, Button{}},
            {GP_B, Button{}},
            {GP_X, Button{}},
            {GP_Y, Button{}},
            {GP_BACK, Button{}},
            {GP_GUIDE, Button{}},
            {GP_START, Button{}},
            {GP_LSTICK, Button{}},
            {GP_RSTICK, Button{}},
            {GP_LSHOULDER, Button{}},
            {GP_RSHOULDER, Button{}},
            {GP_DPAD_UP, Button{}},
            {GP_DPAD_DOWN, Button{}},
            {GP_DPAD_LEFT, Button{}},
            {GP_DPAD_RIGHT, Button{}},
        },
        joystick_id{-1},
        axis_values{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
        joystick_hat_value{HAT_CENTERED}
    {
        SDL_Joystick *joystick =
            SDL_GameControllerGetJoystick(controller.get());
        if (!controller || !joystick)
        {
            throw("Controller or Joystick is a nullptr");
        }

        joystick_id = SDL_JoystickInstanceID(joystick);
        name = std::string{SDL_JoystickName(joystick)};

        auto num_axes = SDL_JoystickNumAxes(joystick);
        auto num_balls = SDL_JoystickNumBalls(joystick);
        auto num_hats = SDL_JoystickNumHats(joystick);
        auto num_buttons = SDL_JoystickNumButtons(joystick);

        ECHO_LOG("Gamepad Name: {}", name);
        ECHO_LOG("Gamepad ID: {}", joystick_id);
        ECHO_LOG("Gamepad Axis: {}", num_axes);
        ECHO_LOG("Gamepad Balls: {}", num_balls);
        ECHO_LOG("Gamepad Hats: {}", num_hats);
        ECHO_LOG("Gamepad Buttons: {}", num_buttons);
    }

    void Gamepad::Update()
    {
        for (auto &[btn, button] : buttons)
        {
            button.Reset();
        }
    }

    void Gamepad::OnBtnPressed(int btn)
    {
        if (btn == GP_UNKNOWN)
        {
            ECHO_ERROR("Gamepad btn [{}] is unknown", btn);
            return;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Gamepad btn [{}] does not exist", btn);
            return;
        }

        itr->second.Update(true);
    }

    void Gamepad::OnBtnReleased(int btn)
    {
        if (btn == GP_UNKNOWN)
        {
            ECHO_ERROR("Gamepad btn [{}] is unknown", btn);
            return;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Gamepad btn [{}] does not exist", btn);
            return;
        }

        itr->second.Update(false);
    }

    bool Gamepad::IsBtnPressed(int btn) const
    {
        if (btn == GP_UNKNOWN)
        {
            ECHO_ERROR("Gamepad btn [{}] is unknown", btn);
            return false;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Gamepad btn [{}] does not exist", btn);
            return false;
        }

        return itr->second.IsPressed;
    }

    bool Gamepad::IsBtnJustPressed(int btn) const
    {
        if (btn == GP_UNKNOWN)
        {
            ECHO_ERROR("Gamepad btn [{}] is unknown", btn);
            return false;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Gamepad btn [{}] does not exist", btn);
            return false;
        }

        return itr->second.JustPressed;
    }

    bool Gamepad::IsBtnJustReleased(int btn) const
    {
        if (btn == GP_UNKNOWN)
        {
            ECHO_ERROR("Gamepad btn [{}] is unknown", btn);
            return false;
        }

        auto itr = buttons.find(btn);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Gamepad btn [{}] does not exist", btn);
            return false;
        }

        return itr->second.JustReleased;
    }

    bool Gamepad::IsGamepadPresent() const
    {
        return controller != nullptr && SDL_NumJoysticks() > 0;
    }

    Sint16 Gamepad::GetAxisPosition(Uint8 axis) const
    {
        auto itr = axis_values.find(axis);
        if (itr == axis_values.end())
        {
            ECHO_ERROR("Axis [{}] does not exist", axis);
            return 0;
        }

        return itr->second;
    }

    void Gamepad::SetAxisPositionValue(Uint8 axis, Sint16 value)
    {
        auto itr = axis_values.find(axis);
        if (itr == axis_values.end())
        {
            ECHO_ERROR("Axis [{}] does not exist", axis);
            return;
        }

        itr->second = value;
    }

    Uint8 Gamepad::GetJoystickHatValue() const
    {
        return joystick_hat_value;
    }

    void Gamepad::SetJoystickHatValue(Uint8 value)
    {
        joystick_hat_value = value;
    }

    bool Gamepad::CheckJoystickID(SDL_JoystickID id) const
    {
        return joystick_id == id;
    }

    const std::string &Gamepad::GetName() const
    {
        return name;
    }
}
