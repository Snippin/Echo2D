#include "Application.h"

#include <Core/ECS/Components/Identity.h>
#include <Core/ECS/Components/SpriteComponent.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/ECS/Entity.h>
#include <Core/Resources/AssetManager.h>
#include <Logger/Logger.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Vertex.h>

#include <glad/glad.h>
#include <SDL.h>

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
        , VAO{0}, VBO{0}, IBO{0}
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
        window = std::make_unique<ECHO_WINDOWING::Window>("Window", 640, 480,
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

        auto asset_manager = std::make_shared<ECHO_RESOURCES::AssetManager>();
        if (!asset_manager)
        {
            ECHO_ERROR("Failed to create `AssetManager`");
            return false;
        }

        if (!asset_manager->AddTexture("hill_tiles",
            "./assets/textures/hill_tiles.png", true))
        {
            ECHO_ERROR("Failed to create and add texture");
            return false;
        }

        const auto &texture = asset_manager->GetTexture("hill_tiles");

        registry = std::make_unique<ECHO_CORE::ECS::Registry>();

        ECHO_CORE::ECS::Entity entity1{*registry, "ent1", "test"};

        const auto &transform = entity1.AddComponent<ECHO_CORE::ECS::TransformComponent>(
            ECHO_CORE::ECS::TransformComponent{
                .Position = glm::vec2{0.f},
                .Scale = glm::vec2{1.f},
                .Rotation = 0.f
            });

        auto &sprite = entity1.AddComponent<ECHO_CORE::ECS::SpriteComponent>(
            ECHO_CORE::ECS::SpriteComponent{
                .Width = 16.f,
                .Height = 16.f,
                .Colour = ECHO_RENDERING::Color{.R = 0,.G = 255,.B = 0,.A = 255},
                .Start_X = 1,
                .Start_Y = 6,
            });

        sprite.GenerateUVs(texture.GetWidth(), texture.GetHeight());

        std::vector<ECHO_RENDERING::Vertex> vertices{};

        ECHO_RENDERING::Vertex top_left{
            glm::vec2{transform.Position.x,
                transform.Position.y + sprite.Height},
            glm::vec2{sprite.Uvs.X, (sprite.Uvs.Y + sprite.Uvs.UV_Height)},
            sprite.Colour
        };

        ECHO_RENDERING::Vertex top_right{
            glm::vec2{transform.Position.x + sprite.Width,
                transform.Position.y + sprite.Height},
            glm::vec2{sprite.Uvs.X + sprite.Uvs.UV_Width,
                sprite.Uvs.Y + sprite.Uvs.UV_Height},
            sprite.Colour
        };

        ECHO_RENDERING::Vertex bot_left{
            glm::vec2{transform.Position.x, transform.Position.y},
            glm::vec2{sprite.Uvs.X , sprite.Uvs.Y},
            sprite.Colour
        };

        ECHO_RENDERING::Vertex bot_right{
            glm::vec2{transform.Position.x + sprite.Width,
                transform.Position.y},
            glm::vec2{sprite.Uvs.X + sprite.Uvs.UV_Width, sprite.Uvs.Y},
            sprite.Colour
        };

        vertices.push_back(top_left);
        vertices.push_back(bot_left);
        vertices.push_back(bot_right);
        vertices.push_back(top_right);

        auto &identity = entity1.GetComponent<ECHO_CORE::ECS::Identity>();
        ECHO_LOG("Name-{}, Group-{}, ID-{}", identity.Name, identity.Group,
            identity.Entity_Id);

        GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Create temp camera
        auto camera = std::make_shared<ECHO_RENDERING::Camera2D>();
        camera->SetScale(10.f);

        if (!registry->AddContext<std::shared_ptr<
            ECHO_RESOURCES::AssetManager>>(asset_manager))
        {
            ECHO_ERROR("Failed to add asset manager to registry context");
            return false;
        }

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

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        // Bind VAO, VBO & IBO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(
            GL_ARRAY_BUFFER,					                // Target buffer type
            vertices.size() * sizeof(ECHO_RENDERING::Vertex),   // size of buffer object data in bytes
            vertices.data(),							        // Pointer to data that will be copied
            GL_STATIC_DRAW						                // Expected usage pattern of data
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,			   // Target buffer type
            6 * sizeof(GLuint),                    // size of buffer object data in bytes
            indices,							   // Pointer to data that will be copied
            GL_STATIC_DRAW						   // Expected usage pattern of data
        );

        glVertexAttribPointer(
            0,					                                // Attribute    -- Layout position in the shader
            2,					                                // Size		    -- Number of components per vertex
            GL_FLOAT,			                                // Type		    -- Data type of components
            GL_FALSE,			                                // Normalized   -- Specifies if fixed-point data values should be normalized
            sizeof(ECHO_RENDERING::Vertex),	                    // Stride       -- Specifies byte offset between consecutive attributes
            (void *)offsetof(ECHO_RENDERING::Vertex, Position) 	// Pointer      -- Specifies the offset of the first component
        );

        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(ECHO_RENDERING::Vertex),
            (void *)offsetof(ECHO_RENDERING::Vertex, UVs)   // Offset of positional data to the first UV coord
        );

        glVertexAttribPointer(
            2,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            sizeof(ECHO_RENDERING::Vertex),
            (void *)offsetof(ECHO_RENDERING::Vertex, Colour)   // Offset of positional data to the first UV coord
        );

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);

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
    }

    void Application::Render()
    {
        const auto &asset_manager = registry->GetContext<std::shared_ptr<
            ECHO_RESOURCES::AssetManager>>();
        const auto &camera = registry->GetContext<std::shared_ptr<
            ECHO_RENDERING::Camera2D>>();

        auto &shader = asset_manager->GetShader("basic");
        auto projection = camera->GetCameraMatrix();

        if (shader.ShaderProgramID() == 0)
        {
            ECHO_ERROR("Shader program has not been created properly");
            return;
        }

        glViewport(0, 0, window->GetWidth(), window->GetHeight());

        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Enable();
        glBindVertexArray(VAO);

        shader.SetUniformMat4("uProjection", projection);

        glActiveTexture(GL_TEXTURE0);
        const auto &texture = asset_manager->GetTexture("hill_tiles");
        glBindTexture(GL_TEXTURE_2D, texture.GetID());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        SDL_GL_SwapWindow(window->GetWindow().get());

        shader.Disable();
    }

    void Application::CleanUp()
    {
        SDL_Quit();
    }
}
