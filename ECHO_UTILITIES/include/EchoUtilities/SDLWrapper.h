#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <memory>

namespace ECHO_UTIL
{
    struct SDLDestroyer
    {
        void operator()(SDL_Window *window) const;
        void operator()(SDL_GameController *controller) const;
        void operator()(SDL_Cursor *cursor) const;
        void operator()(Mix_Chunk *chunk) const;
        void operator()(Mix_Music *music) const;
    };
}

using WindowPtr = std::unique_ptr<SDL_Window, ECHO_UTIL::SDLDestroyer>;
using ControllerPtr = std::shared_ptr<SDL_GameController>;
using CursorPtr = std::shared_ptr<SDL_Cursor>;
using SoundFXPtr = std::unique_ptr<Mix_Chunk, ECHO_UTIL::SDLDestroyer>;
using MusicPtr = std::unique_ptr<Mix_Music, ECHO_UTIL::SDLDestroyer>;

static ControllerPtr MakeSharedController(SDL_GameController *controller)
{
    return std::shared_ptr<SDL_GameController>(
        controller, ECHO_UTIL::SDLDestroyer{});
}

static CursorPtr MakeSharedCursor(SDL_Cursor *cursor)
{
    return CursorPtr();
}
