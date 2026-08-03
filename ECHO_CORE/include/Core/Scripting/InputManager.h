#pragma once

#include <Windowing/Inputs/Gamepad.h>
#include <Windowing/Inputs/Keyboard.h>
#include <Windowing/Inputs/Mouse.h>

#include <SDL.h>
#include <sol/sol.hpp>

#include <map>
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
        std::map<int, std::shared_ptr<ECHO_WINDOW::INPUTS::Gamepad>>
            &GetGamepads();

        std::shared_ptr<ECHO_WINDOW::INPUTS::Gamepad> GetGamepad(int index);
        bool AddGamepad(Sint32 gamepad_index);
        bool RemoveGamepad(Sint32 gamepad_id);

        void GamepadBtnPressed(const SDL_Event &event);
        void GamepadBtnReleased(const SDL_Event &event);
        void GamepadAxisValues(const SDL_Event &event);
        void GamepadHatValues(const SDL_Event &event);

        void UpdateGamepads();

    private:
        InputManager();
        ~InputManager() = default;

        InputManager(const InputManager &) = delete;
        InputManager &operator=(const InputManager &) = delete;

        static void RegisterKeyNames(sol::state &lua);
        static void RegisterMouseBtnNames(sol::state &lua);
        static void RegisterGamepadBtnNames(sol::state &lua);

        std::unique_ptr<ECHO_WINDOW::INPUTS::Keyboard> keyboard;
        std::unique_ptr<ECHO_WINDOW::INPUTS::Mouse> mouse;
        std::map<int, std::shared_ptr<ECHO_WINDOW::INPUTS::Gamepad>> gamepads;
    };
}
