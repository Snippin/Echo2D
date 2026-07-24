#include "Window.h"

#include <iostream>

namespace ECHO_WINDOWING
{
	Window::Window(const std::string &title, int width, int height, int x,
		int y, bool v_sync, Uint32 flags) :
		window{nullptr}, gl_context{}, title{title}, width{width},
		height{height}, x_pos{x}, y_pos{y}, window_flags{flags}
	{
		CreateNewWindow(flags);

		// Enable v_sync
		if (v_sync && !SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
		{
			std::cout << "Failed to enable VSYNC\n";
		}

		std::cout << "Window created successfully\n";
	}

	Window::~Window()
	{
	}

	void Window::SetWindowName(const std::string &name)
	{
		title = name;
		SDL_SetWindowTitle(window.get(), name.c_str());
	}

	void Window::CreateNewWindow(Uint32 flags)
	{
		window = WindowPtr(SDL_CreateWindow(title.c_str(), x_pos, y_pos, width,
			height, flags));

		// Check if window was created correctly
		if (!window)
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to create window: " << error << "\n";
		}
	}
}
