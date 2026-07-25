#define SDL_MAIN_HANDLED = 1;
#define NOMINMAX

#include "Application.h"

int main()
{
    auto &app = ECHO_EDITOR::Application::Get();
    app.Run();

    return 0;
}
