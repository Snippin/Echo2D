#pragma once

#include <EchoUtilities/SDLWrapper.h>
#include <SDL.h>

#include <string>

namespace ECHO_WINDOW
{
    class Window
    {
    public:
        Window() :
            Window("Default Window", 640, 480, SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED, true, NULL)
        {
        }

        Window(const std::string &title, int width, int height, int x, int y,
            bool v_sync = true, Uint32 flags = (SDL_WINDOW_OPENGL |
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE));
        ~Window();

        inline SDL_GLContext &GetGLContext() { return gl_context; }
        inline void SetGLContext(SDL_GLContext context) { gl_context = context; }

        inline WindowPtr &GetWindow() { return window; }
        inline const std::string &GetWindowName() const { return title; }
        inline int GetWidth() const { return width; }
        inline void SetWidth(int width) { this->width = width; }
        inline int GetHeight() const { return height; }
        inline void SetHeight(int height) { this->height = height; }

        inline int GetXPos() const { return x_pos; }
        inline void SetXPos(int x) { x_pos = x; }
        inline int GetYPos() const { return y_pos; }
        inline void SetYPos(int y) { y_pos = y; }

        void SetWindowName(const std::string &name);

    private:
        WindowPtr window;
        SDL_GLContext gl_context;
        std::string title;
        int width;
        int height;
        int x_pos;
        int y_pos;
        Uint32 window_flags;

        void CreateNewWindow(Uint32 flags);
    };
}
