#include "Core/Scripting/InputManager.h"

namespace ECHO_CORE
{
    InputManager &InputManager::Get()
    {
        static InputManager instance;
        return instance;
    }

    ECHO_WINDOW::INPUTS::Keyboard &InputManager::GetKeyboard()
    {
        return *keyboard;
    }

    void InputManager::CreateLuaBind(sol::state &lua)
    {
        RegisterKeyNames(lua);

        auto &input_manager = Get();
        const auto &keyboard = input_manager.GetKeyboard();

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
    }

    InputManager::InputManager() :
        keyboard{std::make_unique<ECHO_WINDOW::INPUTS::Keyboard>()}
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

        lua.set("KP_KEY_0", KEY_KP0);
        lua.set("KP_KEY_1", KEY_KP1);
        lua.set("KP_KEY_2", KEY_KP2);
        lua.set("KP_KEY_3", KEY_KP3);
        lua.set("KP_KEY_4", KEY_KP4);
        lua.set("KP_KEY_5", KEY_KP5);
        lua.set("KP_KEY_6", KEY_KP6);
        lua.set("KP_KEY_7", KEY_KP7);
        lua.set("KP_KEY_8", KEY_KP8);
        lua.set("KP_KEY_9", KEY_KP9);

        lua.set("KP_KEY_DIVIDE", KEY_KPDIVIDE);
        lua.set("KP_KEY_MULTIPLY", KEY_KPMULTIPLY);
        lua.set("KP_KEY_MINUS", KEY_KPMINUS);
        lua.set("KP_KEY_PLUS", KEY_KPPLUS);
        lua.set("KP_KEY_ENTER", KEY_KPENTER);
        lua.set("KP_KEY_PERIOD", KEY_KPPERIOD);
    }
}
