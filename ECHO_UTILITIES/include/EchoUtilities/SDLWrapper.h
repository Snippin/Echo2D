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

typedef std::shared_ptr<SDL_GameController> Controller;
static Controller MakeSharedController(SDL_GameController *controller);

typedef std::shared_ptr<SDL_Cursor> Cursor;
static Controller MakeSharedCursor(SDL_Cursor *cursor);

typedef std::unique_ptr<SDL_Window, ECHO_UTIL::SDLDestroyer> WindowPtr;
