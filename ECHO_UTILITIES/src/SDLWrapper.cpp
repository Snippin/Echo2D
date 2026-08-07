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
}
