#pragma once

#include <SDL_gamecontroller.h>
#include <SDL_joystick.h>

constexpr int GP_UNKNOWN = -1;

// Action buttons
constexpr int GP_A = SDL_CONTROLLER_BUTTON_A;
constexpr int GP_B = SDL_CONTROLLER_BUTTON_B;
constexpr int GP_X = SDL_CONTROLLER_BUTTON_X;
constexpr int GP_Y = SDL_CONTROLLER_BUTTON_Y;

// Menu buttons
constexpr int GP_BACK = SDL_CONTROLLER_BUTTON_BACK;
constexpr int GP_GUIDE = SDL_CONTROLLER_BUTTON_GUIDE;
constexpr int GP_START = SDL_CONTROLLER_BUTTON_START;

// Joystick/Trigger buttons
constexpr int GP_LSTICK = SDL_CONTROLLER_BUTTON_LEFTSTICK;
constexpr int GP_RSTICK = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
constexpr int GP_LSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
constexpr int GP_RSHOULDER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;

// Directional pads
constexpr int GP_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP;
constexpr int GP_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
constexpr int GP_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
constexpr int GP_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

// Joystick
constexpr int HAT_CENTERED = SDL_HAT_CENTERED;
constexpr int HAT_UP = SDL_HAT_UP;
constexpr int HAT_RIGHT = SDL_HAT_RIGHT;
constexpr int HAT_DOWN = SDL_HAT_DOWN;
constexpr int HAT_LEFT = SDL_HAT_LEFT;
constexpr int HAT_RIGHTUP = SDL_HAT_RIGHTUP;
constexpr int HAT_RIGHTDOWN = SDL_HAT_RIGHTDOWN;
constexpr int HAT_LEFTUP = SDL_HAT_LEFTUP;
constexpr int HAT_LEFTDOWN = SDL_HAT_LEFTDOWN;
