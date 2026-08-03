#pragma once

#include <Windowing/Inputs/Keyboard.h>

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

    private:
        InputManager();
        ~InputManager() = default;

        InputManager(const InputManager &) = delete;
        InputManager &operator=(const InputManager &) = delete;

        static void RegisterKeyNames(sol::state &lua);

        std::unique_ptr<ECHO_WINDOW::INPUTS::Keyboard> keyboard;
    };
}
