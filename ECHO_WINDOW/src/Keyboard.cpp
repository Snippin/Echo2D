#include "Windowing/Inputs/Keyboard.h"

#include <Logger/Logger.h>

namespace ECHO_WINDOW::INPUTS
{
    static bool IsKeyValid(int key)
    {
    }

    Keyboard::Keyboard() :
        buttons{
            {KEY_BACKSPACE, Button{}},
            {KEY_TAB, Button{}},
            {KEY_CLEAR, Button{}},
            {KEY_RETURN, Button{}},
            {KEY_PAUSE, Button{}},
            {KEY_ESCAPE, Button{}},
            {KEY_SPACE, Button{}},
            {KEY_EXCLAIM, Button{}},
            {KEY_QUOTEDBL, Button{}},
            {KEY_HASH, Button{}},
            {KEY_DOLLAR, Button{}},
            {KEY_AMPERSAND, Button{}},
            {KEY_QUOTE, Button{}},
            {KEY_LEFTPAREN, Button{}},
            {KEY_RIGHTPAREN, Button{}},
            {KEY_ASTERISK, Button{}},
            {KEY_PLUS, Button{}},
            {KEY_COMMA, Button{}},
            {KEY_MINUS, Button{}},
            {KEY_PERIOD, Button{}},
            {KEY_SLASH, Button{}},
            {KEY_0, Button{}},
            {KEY_1, Button{}},
            {KEY_2, Button{}},
            {KEY_3, Button{}},
            {KEY_4, Button{}},
            {KEY_5, Button{}},
            {KEY_6, Button{}},
            {KEY_7, Button{}},
            {KEY_8, Button{}},
            {KEY_9, Button{}},
            {KEY_COLON, Button{}},
            {KEY_SEMICOLON, Button{}},
            {KEY_LESS, Button{}},
            {KEY_EQUALS, Button{}},
            {KEY_GREATER, Button{}},
            {KEY_QUESTION, Button{}},
            {KEY_AT, Button{}},
            {KEY_LEFTBRACKET, Button{}},
            {KEY_BACKSLASH, Button{}},
            {KEY_RIGHTBRACKET, Button{}},
            {KEY_CARET, Button{}},
            {KEY_UNDERSCORE, Button{}},
            {KEY_BACKQUOTE, Button{}},
            {KEY_A, Button{}},
            {KEY_B, Button{}},
            {KEY_C, Button{}},
            {KEY_D, Button{}},
            {KEY_E, Button{}},
            {KEY_F, Button{}},
            {KEY_G, Button{}},
            {KEY_H, Button{}},
            {KEY_I, Button{}},
            {KEY_J, Button{}},
            {KEY_K, Button{}},
            {KEY_L, Button{}},
            {KEY_M, Button{}},
            {KEY_N, Button{}},
            {KEY_O, Button{}},
            {KEY_P, Button{}},
            {KEY_Q, Button{}},
            {KEY_R, Button{}},
            {KEY_S, Button{}},
            {KEY_T, Button{}},
            {KEY_U, Button{}},
            {KEY_V, Button{}},
            {KEY_W, Button{}},
            {KEY_X, Button{}},
            {KEY_Y, Button{}},
            {KEY_Z, Button{}},
            {KEY_DELETE, Button{}},
            {KEY_CAPSLOCK, Button{}},
            {KEY_F1, Button{}},
            {KEY_F2, Button{}},
            {KEY_F3, Button{}},
            {KEY_F4, Button{}},
            {KEY_F5, Button{}},
            {KEY_F6, Button{}},
            {KEY_F7, Button{}},
            {KEY_F8, Button{}},
            {KEY_F9, Button{}},
            {KEY_F10, Button{}},
            {KEY_F11, Button{}},
            {KEY_F12, Button{}},
            {KEY_PRINTSCREEN, Button{}},
            {KEY_SCROLLLOCK, Button{}},
            {KEY_INSERT, Button{}},
            {KEY_HOME, Button{}},
            {KEY_PAGEUP, Button{}},
            {KEY_END, Button{}},
            {KEY_PAGEDOWN, Button{}},
            {KEY_RIGHT, Button{}},
            {KEY_LEFT, Button{}},
            {KEY_DOWN, Button{}},
            {KEY_UP, Button{}},
            {KEY_NUMLOCK, Button{}},
            {KEY_KPDIVIDE, Button{}},
            {KEY_KPMULTIPLY, Button{}},
            {KEY_KPMINUS, Button{}},
            {KEY_KPPLUS, Button{}},
            {KEY_KPENTER, Button{}},
            {KEY_KP0, Button{}},
            {KEY_KP1, Button{}},
            {KEY_KP2, Button{}},
            {KEY_KP3, Button{}},
            {KEY_KP4, Button{}},
            {KEY_KP5, Button{}},
            {KEY_KP6, Button{}},
            {KEY_KP7, Button{}},
            {KEY_KP8, Button{}},
            {KEY_KP9, Button{}},
            {KEY_KPPERIOD, Button{}},
            {KEY_LCTRL, Button{}},
            {KEY_LSHIFT, Button{}},
            {KEY_LALT, Button{}},
            {KEY_RCTRL, Button{}},
            {KEY_RSHIFT, Button{}},
            {KEY_RALT, Button{}},
            {KEY_LGUI , Button{}},
            {KEY_RGUI , Button{}}}
    {
    }

    void Keyboard::Update()
    {
        for (auto &[key, button] : buttons)
        {
            button.Reset();
        }
    }

    void Keyboard::OnKeyPressed(int key)
    {
        if (key == KEY_UKNOWN)
        {
            ECHO_ERROR("Key [{}] is unknown", key);
            return;
        }

        auto itr = buttons.find(key);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Key [{}] does not exist", key);
            return;
        }

        itr->second.Update(true);
    }

    void Keyboard::OnKeyReleased(int key)
    {
        if (key == KEY_UKNOWN)
        {
            ECHO_ERROR("Key [{}] is unknown", key);
            return;
        }

        auto itr = buttons.find(key);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Key [{}] does not exist", key);
            return;
        }

        itr->second.Update(false);
    }

    bool Keyboard::IsKeyPressed(int key) const
    {
        if (key == KEY_UKNOWN)
        {
            ECHO_ERROR("Key [{}] is unknown", key);
            return false;
        }

        auto itr = buttons.find(key);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Key [{}] does not exist", key);
            return false;
        }

        return itr->second.IsPressed;
    }

    bool Keyboard::IsKeyJustPressed(int key) const
    {
        if (key == KEY_UKNOWN)
        {
            ECHO_ERROR("Key [{}] is unknown", key);
            return false;
        }

        auto itr = buttons.find(key);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Key [{}] does not exist", key);
            return false;
        }

        return itr->second.JustPressed;
    }

    bool Keyboard::IsKeyJustReleased(int key) const
    {
        if (key == KEY_UKNOWN)
        {
            ECHO_ERROR("Key [{}] is unknown", key);
            return false;
        }

        auto itr = buttons.find(key);
        if (itr == buttons.end())
        {
            ECHO_ERROR("Key [{}] does not exist", key);
            return false;
        }

        return itr->second.JustReleased;
    }
}
