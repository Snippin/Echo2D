#pragma once

#include <Core/ECS/Registry.h>
#include <Windowing/Window/Window.h>

#include <glad/glad.h>
#include <memory>

namespace ECHO_EDITOR
{
    class Application
    {
    public:
        static Application &Get();
        ~Application();

        void Run();

    private:
        Application();

        std::unique_ptr<ECHO_WINDOWING::Window> window;
        std::unique_ptr<ECHO_CORE::ECS::Registry> registry;

        SDL_Event event;
        bool running;

        // TODO: Temporary for test
        GLuint VAO;
        GLuint VBO;
        GLuint IBO;

        bool Initialise();
        bool LoadShaders();

        void ProcessEvents();
        void Update();
        void Render();

        void CleanUp();
    };
}
