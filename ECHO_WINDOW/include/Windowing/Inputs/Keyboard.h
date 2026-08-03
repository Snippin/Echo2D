#pragma once

#include "Button.h"
#include "Keys.h"

#include <map>

namespace ECHO_WINDOW::INPUTS
{
    class Keyboard
    {
    public:
        Keyboard();
        ~Keyboard() = default;

        void Update();

        void OnKeyPressed(int key);
        void OnKeyReleased(int key);

        bool IsKeyPressed(int key) const;
        bool IsKeyJustPressed(int key) const;
        bool IsKeyJustReleased(int key) const;

    private:
        std::map<int, Button> buttons;
    };
}
