#include "EchoUtilities/SDLWrapper.h"

#include <iostream>

namespace ECHO_UTIL
{
    void SDLDestroyer::operator()(SDL_Window *window) const
    {
        SDL_DestroyWindow(window);
        std::cout << "Destroyed SDL WINDOW\n";
    }

    void SDLDestroyer::operator()(SDL_GameController *controller) const
    {
        SDL_GameControllerClose(controller);
        controller = nullptr;
        std::cout << "Closed SDL_GameController\n";
    }

    void SDLDestroyer::operator()(SDL_Cursor *cursor) const
    {
    }

    void SDLDestroyer::operator()(Mix_Chunk *chunk) const
    {
        Mix_FreeChunk(chunk);
        std::cout << "Freed SDL `Mix_Chunk`\n";
    }

    void SDLDestroyer::operator()(Mix_Music *music) const
    {
        Mix_FreeMusic(music);
        std::cout << "Freed SDL `Mix_Music`\n";
    }
}
