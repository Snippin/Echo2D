#include "Application.h"

#include <Core/ECS/Entity.h>
#include <Core/Resources/AssetManager.h>
#include <Core/Scripting/InputManager.h>
#include <Core/Systems/AnimationSystem.h>
#include <Core/Systems/RenderSystem.h>
#include <Core/Systems/ScriptSystem.h>
#include <Logger/Logger.h>
#include <Rendering/Core/Camera2D.h>

#include <glad/glad.h>
#include <SDL.h>
#include <sol/sol.hpp>

namespace ECHO_EDITOR
{
    Application &Application::Get()
    {
        static Application instance{};
        return instance;
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        if (!Initialise())
        {
            ECHO_ERROR("Failed to initialise");
            return;
        }

        while (running)
        {
            ProcessEvents();
            Update();
            Render();
        }

        CleanUp();
    }

    Application::Application() :
        window{nullptr}, registry{nullptr}, event{}, running{true}
    {
    }

    bool Application::Initialise()
    {
        ECHO_INIT_LOGS(true, true);

        // Initialise SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            std::string error = SDL_GetError();
            ECHO_ERROR("Failed to initialise SDL: {}", error);
            return false;
        }

        // Initialise OpenGL
        if (SDL_GL_LoadLibrary(NULL) != 0)
        {
            std::string error = SDL_GetError();
            ECHO_ERROR("Failed to open GL Library: {}", error);
            return false;
        }

        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);

        // Set number of bits per channel
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        // Create window
        window = std::make_unique<ECHO_WINDOW::Window>("Window", 640, 480,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true,
            SDL_WINDOW_OPENGL);

        if (!window->GetWindow())
        {
            ECHO_ERROR("Failed to create window");
            return false;
        }

        // Create OpenGL context
        window->SetGLContext(SDL_GL_CreateContext(window->GetWindow().get()));

        if (!window->GetGLContext())
        {
            std::string error = SDL_GetError();
            ECHO_ERROR("Failed to create OpenGL Context: {}", error);
            return false;
        }

        SDL_GL_MakeCurrent(window->GetWindow().get(), window->GetGLContext());
        SDL_GL_SetSwapInterval(1);

        // Initialise GLAD
        if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
        {
            ECHO_ERROR("Failed to GLAD");
            return false;
        }

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        registry = std::make_unique<ECHO_CORE::ECS::Registry>();

        auto asset_manager = std::make_shared<ECHO_RESOURCES::AssetManager>();
        if (!asset_manager)
        {
            ECHO_ERROR("Failed to create `AssetManager`");
            return false;
        }

        if (!registry->AddContext<std::shared_ptr<
            ECHO_RESOURCES::AssetManager>>(asset_manager))
        {
            ECHO_ERROR("Failed to add asset manager to registry context");
            return false;
        }

        if (!asset_manager->AddTexture("hill_tiles",
            "./assets/textures/hill_tiles.png", true))
        {
            ECHO_ERROR("Failed to create and add texture");
            return false;
        }

        if (!asset_manager->AddTexture("warrior_princess",
            "./assets/textures/warrior_princess.png", true))
        {
            ECHO_ERROR("Failed to create and add texture");
            return false;
        }

        // Create lua state
        auto lua = std::make_shared<sol::state>();

        if (!lua)
        {
            ECHO_ERROR("Failed to create lua state");
            return false;
        }

        lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os,
            sol::lib::table, sol::lib::io, sol::lib::string);

        if (!registry->AddContext<std::shared_ptr<sol::state>>(lua))
        {
            ECHO_ERROR("Failed to add sol::state to registry context");
            return false;
        }

        // Create script system
        auto script_system = std::make_shared<
            ECHO_CORE::SYSTEMS::ScriptSystem>(*registry);

        if (!script_system)
        {
            ECHO_ERROR("Failed to create script system");
            return false;
        }

        if (!registry->AddContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::ScriptSystem>>(script_system))
        {
            ECHO_ERROR("Failed to add script system to registry context");
            return false;
        }

        // Create render system
        auto render_system = std::make_shared<
            ECHO_CORE::SYSTEMS::RenderSystem>(*registry);

        if (!render_system)
        {
            ECHO_ERROR("Failed to create render system");
            return false;
        }

        if (!registry->AddContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::RenderSystem>>(render_system))
        {
            ECHO_ERROR("Failed to add render system to registry context");
            return false;
        }

        // Create animation system
        auto animation_system = std::make_shared<
            ECHO_CORE::SYSTEMS::AnimationSystem>(*registry);

        if (!animation_system)
        {
            ECHO_ERROR("Failed to create animation system");
            return false;
        }

        if (!registry->AddContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::AnimationSystem>>(animation_system))
        {
            ECHO_ERROR("Failed to add animation system to registry context");
            return false;
        }

        // Create camera
        auto camera = std::make_shared<ECHO_RENDERING::Camera2D>();

        if (!registry->AddContext<std::shared_ptr<ECHO_RENDERING::Camera2D>>(
            camera))
        {
            ECHO_ERROR("Failed to add camera to registry context");
            return false;
        }

        if (!LoadShaders())
        {
            ECHO_ERROR("Failed to load shaders");
            return false;
        }

        ECHO_CORE::SYSTEMS::ScriptSystem::RegisterLuaBindings(*lua, *registry);

        if (!script_system->LoadMainScript(*lua))
        {
            ECHO_ERROR("Failed to load main lua script");
            return false;
        }

        return true;
    }

    bool Application::LoadShaders()
    {
        const auto &asset_manager = registry->GetContext<std::shared_ptr<
            ECHO_RESOURCES::AssetManager>>();

        if (!asset_manager)
        {
            ECHO_ERROR("Failed to get asset manager from registry");
            return false;
        }

        // Create shader
        if (!asset_manager->AddShader("basic",
            "./assets/shaders/basic_shader"))
        {
            ECHO_ERROR("Failed to create and add shader");
            return false;
        }

        return true;
    }

    void Application::ProcessEvents()
    {
        auto &input_manager = ECHO_CORE::InputManager::Get();
        auto &keyboard = input_manager.GetKeyboard();
        auto &mouse = input_manager.GetMouse();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }

                keyboard.OnKeyPressed(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                keyboard.OnKeyReleased(event.key.keysym.sym);
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouse.OnBtnPressed(event.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                mouse.OnBtnReleased(event.button.button);
                break;

            case SDL_MOUSEWHEEL:
                mouse.SetMouseWheelX(event.wheel.x);
                mouse.SetMouseWheelY(event.wheel.y);
                break;

            case SDL_MOUSEMOTION:
                mouse.SetMouseMoving(true);
                break;

            default:
                break;
            }
        }
    }

    void Application::Update()
    {
        const auto &camera = registry->GetContext<std::shared_ptr<
            ECHO_RENDERING::Camera2D>>();

        if (!camera)
        {
            ECHO_ERROR("Failed to get camera from registry context");
            return;
        }

        camera->Update();

        const auto &script_system = registry->GetContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::ScriptSystem>>();
        script_system->Update();

        const auto &animation_system = registry->GetContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::AnimationSystem>>();
        animation_system->Update();

        auto &input_manager = ECHO_CORE::InputManager::Get();

        auto &keyboard = input_manager.GetKeyboard();
        keyboard.Update();

        auto &mouse = input_manager.GetMouse();
        mouse.Update();
    }

    void Application::Render()
    {
        const auto &render_system = registry->GetContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::RenderSystem>>();
        const auto &script_system = registry->GetContext<std::shared_ptr<
            ECHO_CORE::SYSTEMS::ScriptSystem>>();

        glViewport(0, 0, window->GetWidth(), window->GetHeight());

        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        script_system->Render();
        render_system->Render();

        SDL_GL_SwapWindow(window->GetWindow().get());
    }

    void Application::CleanUp()
    {
        SDL_Quit();
    }
}
