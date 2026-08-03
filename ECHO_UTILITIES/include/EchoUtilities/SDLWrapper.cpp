#include "SDLWrapper.h"

#include <iostream>

void ECHO_UTIL::SDLDestroyer::operator()(SDL_Window *window) const
{
	SDL_DestroyWindow(window);
	std::cout << "Destroyed SDL WINDOW\n";
}

void ECHO_UTIL::SDLDestroyer::operator()(SDL_GameController *controller) const
{
}

void ECHO_UTIL::SDLDestroyer::operator()(SDL_Cursor *cursor) const
{
}

Controller MakeSharedController(SDL_GameController *controller)
{
	return Controller();
}

Cursor MakeSharedCursor(SDL_Cursor *cursor)
{
    return Cursor();
}
