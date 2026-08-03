#include "Core/Scripting/InputManager.h"
#include "Windowing/Inputs/GamepadButtons.h"
#include "Windowing/Inputs/MouseButtons.h"

#include <EchoUtilities/SDLWrapper.h>
#include <Logger/Logger.h>

static constexpr int MAX_CONTROLLERS = 4;

namespace ECHO_CORE
{
    InputManager &InputManager::Get()
    {
        static InputManager instance;
        return instance;
    }

    void InputManager::CreateLuaBind(sol::state &lua)
    {
        RegisterKeyNames(lua);
        RegisterMouseBtnNames(lua);
        RegisterGamepadBtnNames(lua);

        auto &input_manager = Get();
        const auto &keyboard = input_manager.GetKeyboard();
        auto &mouse = input_manager.GetMouse();

        lua.new_usertype<ECHO_WINDOW::INPUTS::Keyboard>(
            "Keyboard",
            sol::no_constructor,
            "Pressed",
            [&keyboard](int key) { return keyboard.IsKeyPressed(key); },
            "JustPressed",
            [&keyboard](int key) { return keyboard.IsKeyJustPressed(key); },
            "JustReleased",
            [&keyboard](int key) { return keyboard.IsKeyJustReleased(key); }
        );

        lua.new_usertype<ECHO_WINDOW::INPUTS::Mouse>(
            "Mouse",
            sol::no_constructor,
            "Pressed",
            [&mouse](int btn) { return mouse.IsBtnPressed(btn); },
            "JustPressed",
            [&mouse](int btn) { return mouse.IsBtnJustPressed(btn); },
            "JustReleased",
            [&mouse](int btn) { return mouse.IsBtnJustReleased(btn); },
            "ScreenPosition",
            [&mouse]() { return mouse.GetMouseScreenPosition(); },
            "WheelX",
            [&mouse]() { return mouse.GetMouseWheelX(); },
            "WheelY",
            [&mouse]() { return mouse.GetMouseWheelY(); }
        );

        lua.new_usertype<ECHO_WINDOW::INPUTS::Gamepad>(
            "Gamepad",
            sol::no_constructor,
            "Pressed",
            [&input_manager](int index, int btn)
            {
                auto gamepad = input_manager.GetGamepad(index);
                if (!gamepad)
                {
                    ECHO_ERROR("Invalid gamepad index [{}] or not plugged in",
                        index);
                    return false;
                }

                return gamepad->IsBtnPressed(btn);
            },
            "JustPressed",
            [&input_manager](int index, int btn)
            {
                auto gamepad = input_manager.GetGamepad(index);
                if (!gamepad)
                {
                    ECHO_ERROR("Invalid gamepad index [{}] or not plugged in",
                        index);
                    return false;
                }

                return gamepad->IsBtnJustPressed(btn);
            },
            "JustReleased",
            [&input_manager](int index, int btn)
            {
                auto gamepad = input_manager.GetGamepad(index);
                if (!gamepad)
                {
                    ECHO_ERROR("Invalid gamepad index [{}] or not plugged in",
                        index);
                    return false;
                }

                return gamepad->IsBtnJustReleased(btn);
            },
            "GetAxisPosition",
            [&input_manager](int index, int axis)
            {
                auto gamepad = input_manager.GetGamepad(index);
                if (!gamepad)
                {
                    ECHO_ERROR("Invalid gamepad index [{}] or not plugged in",
                        index);
                    return Sint16{0};
                }

                return gamepad->GetAxisPosition(axis);
            },
            "GetHatValue",
            [&input_manager](int index)
            {
                auto gamepad = input_manager.GetGamepad(index);
                if (!gamepad)
                {
                    ECHO_ERROR("Invalid gamepad index [{}] or not plugged in",
                        index);
                    return Uint8{0};
                }

                return gamepad->GetJoystickHatValue();
            }
        );
    }

    ECHO_WINDOW::INPUTS::Keyboard &InputManager::GetKeyboard()
    {
        return *keyboard;
    }

    ECHO_WINDOW::INPUTS::Mouse &InputManager::GetMouse()
    {
        return *mouse;
    }

    std::map<int, std::shared_ptr<ECHO_WINDOW::INPUTS::Gamepad>>
        &InputManager::GetGamepads()
    {
        return gamepads;
    }

    std::shared_ptr<ECHO_WINDOW::INPUTS::Gamepad> InputManager::GetGamepad(
        int index)
    {
        auto itr = gamepads.find(index);
        if (itr == gamepads.end())
        {
            ECHO_ERROR("Failed to get gamepad at [{}]", index);
            return nullptr;
        }

        return itr->second;
    }

    bool InputManager::AddGamepad(Sint32 gamepad_index)
    {
        if (gamepads.size() >= MAX_CONTROLLERS)
        {
            ECHO_ERROR("Adding too many controllers. "
                "Max {} controllers allowed", MAX_CONTROLLERS);
            return false;
        }

        std::shared_ptr<ECHO_WINDOW::INPUTS::Gamepad> gamepad{nullptr};
        try
        {
            gamepad = std::make_shared<ECHO_WINDOW::INPUTS::Gamepad>(
                std::move(MakeSharedController(
                    SDL_GameControllerOpen(gamepad_index)))
            );
        }
        catch (...)
        {
            std::string error{SDL_GetError()};
            ECHO_ERROR("Failed to open gamepad - {}", error);
            return false;
        }

        for (int i = 0; i < MAX_CONTROLLERS; i++)
        {
            if (gamepads.contains(i))
            {
                continue;
            }

            gamepads.emplace(i, std::move(gamepad));
            ECHO_LOG("Gamepad [{}] added at index [{}]", gamepad_index, i);
            return true;
        }

        assert(false && "Failed to add new gamepad");
        ECHO_ERROR("Failed to add new gamepad");
        return false;
    }

    bool InputManager::RemoveGamepad(Sint32 gamepad_id)
    {
        auto gamepad_removed = std::erase_if(gamepads,
            [&gamepad_id](auto &gamepad)
            {
                return gamepad.second->CheckJoystickID(gamepad_id);
            }
        );

        if (gamepad_removed > 0)
        {
            ECHO_LOG("Gamepad removed - [{}]", gamepad_id);
            return true;
        }

        assert(false && "Failed to remove gamepad - has not been mapped");
        ECHO_ERROR("Failed to remove gamepad - ID [{}] has not been mapped",
            gamepad_id);
        return false;
    }

    void InputManager::GamepadBtnPressed(const SDL_Event &event)
    {
        for (const auto &[index, gamepad] : gamepads)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->OnBtnPressed(event.cbutton.button);
                break;
            }
        }
    }

    void InputManager::GamepadBtnReleased(const SDL_Event &event)
    {
        for (const auto &[index, gamepad] : gamepads)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->OnBtnReleased(event.cbutton.button);
                break;
            }
        }
    }

    void InputManager::GamepadAxisValues(const SDL_Event &event)
    {
        for (const auto &[index, gamepad] : gamepads)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->SetAxisPositionValue(event.jaxis.axis,
                    event.jaxis.value);
                break;
            }
        }
    }

    void InputManager::GamepadHatValues(const SDL_Event &event)
    {
        for (const auto &[index, gamepad] : gamepads)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->SetJoystickHatValue(event.jhat.value);
                break;
            }
        }
    }

    void InputManager::UpdateGamepads()
    {
        for (const auto &[index, gamepad] : gamepads)
        {
            if (gamepad)
            {
                gamepad->Update();
            }
        }
    }

    InputManager::InputManager() :
        keyboard{std::make_unique<ECHO_WINDOW::INPUTS::Keyboard>()},
        mouse{std::make_unique<ECHO_WINDOW::INPUTS::Mouse>()}
    {
    }

    void InputManager::RegisterKeyNames(sol::state &lua)
    {
        lua.set("KEY_A", KEY_A);
        lua.set("KEY_B", KEY_B);
        lua.set("KEY_C", KEY_C);
        lua.set("KEY_D", KEY_D);
        lua.set("KEY_E", KEY_E);
        lua.set("KEY_F", KEY_F);
        lua.set("KEY_G", KEY_G);
        lua.set("KEY_H", KEY_H);
        lua.set("KEY_I", KEY_I);
        lua.set("KEY_J", KEY_J);
        lua.set("KEY_K", KEY_K);
        lua.set("KEY_L", KEY_L);
        lua.set("KEY_M", KEY_M);
        lua.set("KEY_N", KEY_N);
        lua.set("KEY_O", KEY_O);
        lua.set("KEY_P", KEY_P);
        lua.set("KEY_Q", KEY_Q);
        lua.set("KEY_R", KEY_R);
        lua.set("KEY_S", KEY_S);
        lua.set("KEY_T", KEY_T);
        lua.set("KEY_U", KEY_U);
        lua.set("KEY_V", KEY_V);
        lua.set("KEY_W", KEY_W);
        lua.set("KEY_X", KEY_X);
        lua.set("KEY_Y", KEY_Y);
        lua.set("KEY_Z", KEY_Z);

        lua.set("KEY_0", KEY_0);
        lua.set("KEY_1", KEY_1);
        lua.set("KEY_2", KEY_2);
        lua.set("KEY_3", KEY_3);
        lua.set("KEY_4", KEY_4);
        lua.set("KEY_5", KEY_5);
        lua.set("KEY_6", KEY_6);
        lua.set("KEY_7", KEY_7);
        lua.set("KEY_8", KEY_8);
        lua.set("KEY_9", KEY_9);

        lua.set("KEY_ENTER", KEY_RETURN);
        lua.set("KEY_BACKSPACE", KEY_BACKSPACE);
        lua.set("KEY_ESC", KEY_ESCAPE);
        lua.set("KEY_SPACE", KEY_SPACE);
        lua.set("KEY_LCTRL", KEY_LCTRL);
        lua.set("KEY_RCTRL", KEY_RCTRL);
        lua.set("KEY_LALT", KEY_LALT);
        lua.set("KEY_RALT", KEY_RALT);
        lua.set("KEY_LSHIFT", KEY_LSHIFT);
        lua.set("KEY_RSHIFT", KEY_RSHIFT);

        lua.set("KEY_COLON", KEY_COLON);
        lua.set("KEY_SEMICOLON", KEY_SEMICOLON);
        lua.set("KEY_QUOTE", KEY_QUOTE);
        lua.set("KEY_BACKQUOTE", KEY_BACKQUOTE);
        lua.set("KEY_CARET", KEY_CARET);
        lua.set("KEY_UNDERSCORE", KEY_UNDERSCORE);
        lua.set("KEY_RIGHTBRACKET", KEY_RIGHTBRACKET);
        lua.set("KEY_LEFTBRACKET", KEY_LEFTBRACKET);
        lua.set("KEY_SLASH", KEY_SLASH);
        lua.set("KEY_ASTERISK", KEY_ASTERISK);
        lua.set("KEY_LEFTPAREN", KEY_LEFTPAREN);
        lua.set("KEY_RIGHTPAREN", KEY_RIGHTPAREN);
        lua.set("KEY_QUESTION", KEY_QUESTION);
        lua.set("KEY_AMPERSAND", KEY_AMPERSAND);
        lua.set("KEY_DOLLAR", KEY_DOLLAR);
        lua.set("KEY_EXCLAIM", KEY_EXCLAIM);
        lua.set("KEY_BACKSLASH", KEY_BACKSLASH);

        lua.set("KEY_UP", KEY_UP);
        lua.set("KEY_RIGHT", KEY_RIGHT);
        lua.set("KEY_DOWN", KEY_DOWN);
        lua.set("KEY_LEFT", KEY_LEFT);

        lua.set("KEY_NUM_LOCK", KEY_NUMLOCK);

        lua.set("KEY_KP0", KEY_KP0);
        lua.set("KEY_KP1", KEY_KP1);
        lua.set("KEY_KP2", KEY_KP2);
        lua.set("KEY_KP3", KEY_KP3);
        lua.set("KEY_KP4", KEY_KP4);
        lua.set("KEY_KP5", KEY_KP5);
        lua.set("KEY_KP6", KEY_KP6);
        lua.set("KEY_KP7", KEY_KP7);
        lua.set("KEY_KP8", KEY_KP8);
        lua.set("KEY_KP9", KEY_KP9);

        lua.set("KEY_KPDIVIDE", KEY_KPDIVIDE);
        lua.set("KEY_KPMULTIPLY", KEY_KPMULTIPLY);
        lua.set("KEY_KPMINUS", KEY_KPMINUS);
        lua.set("KEY_KPPLUS", KEY_KPPLUS);
        lua.set("KEY_KPENTER", KEY_KPENTER);
        lua.set("KEY_KPPERIOD", KEY_KPPERIOD);
    }

    void InputManager::RegisterMouseBtnNames(sol::state &lua)
    {
        lua.set("MOUSE_LEFT", MOUSE_LEFT);
        lua.set("MOUSE_MIDDLE", MOUSE_MIDDLE);
        lua.set("MOUSE_RIGHT", MOUSE_RIGHT);
    }

    void InputManager::RegisterGamepadBtnNames(sol::state &lua)
    {
        lua.set("GP_A", GP_A);
        lua.set("GP_B", GP_B);
        lua.set("GP_X", GP_X);
        lua.set("GP_Y", GP_Y);

        lua.set("GP_BACK", GP_BACK);
        lua.set("GP_GUIDE", GP_GUIDE);
        lua.set("GP_START", GP_START);

        lua.set("GP_LSTICK", GP_LSTICK);
        lua.set("GP_RSTICK", GP_RSTICK);
        lua.set("GP_LSHOULDER", GP_LSHOULDER);
        lua.set("GP_RSHOULDER", GP_RSHOULDER);

        lua.set("GP_DPAD_UP", GP_DPAD_UP);
        lua.set("GP_DPAD_DOWN", GP_DPAD_DOWN);
        lua.set("GP_DPAD_LEFT", GP_DPAD_LEFT);
        lua.set("GP_DPAD_RIGHT", GP_DPAD_RIGHT);

        lua.set("AXIS_X1", 0);
        lua.set("AXIS_Y1", 1);
        lua.set("AXIS_X2", 2);
        lua.set("AXIS_Y2", 3);

        // Bottom triggers
        lua.set("AXIS_Z1", 4);
        lua.set("AXIS_Z2", 5);
    }
}
