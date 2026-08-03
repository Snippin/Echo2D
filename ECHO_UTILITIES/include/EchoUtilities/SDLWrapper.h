#pragma once

#include <SDL.h>

#include <memory>

namespace ECHO_UTIL
{
    struct SDLDestroyer
    {
        void operator()(SDL_Window *window) const;
        void operator()(SDL_GameController *controller) const;
        void operator()(SDL_Cursor *cursor) const;
    };
}

using WindowPtr = std::unique_ptr<SDL_Window, ECHO_UTIL::SDLDestroyer>;
using ControllerPtr = std::shared_ptr<SDL_GameController>;
using CursorPtr = std::shared_ptr<SDL_Cursor>;

static ControllerPtr MakeSharedController(SDL_GameController *controller)
{
    return std::shared_ptr<SDL_GameController>(
        controller, ECHO_UTIL::SDLDestroyer{});
}

static CursorPtr MakeSharedCursor(SDL_Cursor *cursor)
{
    return CursorPtr();
}
