#pragma once

#include <Windowing/Inputs/Keyboard.h>
#include <Windowing/Inputs/Mouse.h>

#include <sol/sol.hpp>

#include <memory>

namespace ECHO_CORE
{
    // Singleton
    class InputManager
    {
    public:
        static InputManager &Get();
        static void CreateLuaBind(sol::state &lua);

        ECHO_WINDOW::INPUTS::Keyboard &GetKeyboard();
        ECHO_WINDOW::INPUTS::Mouse &GetMouse();

    private:
        InputManager();
        ~InputManager() = default;

        InputManager(const InputManager &) = delete;
        InputManager &operator=(const InputManager &) = delete;

        static void RegisterKeyNames(sol::state &lua);
        static void RegisterMouseBtnNames(sol::state &lua);

        std::unique_ptr<ECHO_WINDOW::INPUTS::Keyboard> keyboard;
        std::unique_ptr<ECHO_WINDOW::INPUTS::Mouse> mouse;
    };
}
