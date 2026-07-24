#define SDL_MAIN_HANDLED = 1;

#include <Logger/Logger.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/Vertex.h>
#include <Windowing/Window/Window.h>

#include <glad/glad.h>
#include <SDL.h>

#include <iostream>

struct UVs
{
    float X;
    float Y;
    float Width;
    float Height;

    UVs() :
        X{0.f}, Y{0.f}, Width{0.f}, Height{0.f}
    {
    }
};

int main()
{
    ECHO_INIT_LOGS(true, true);

    // Initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::string error = SDL_GetError();
        ECHO_ERROR("Failed to initialise SDL: {}", error);
        return -1;
    }

    // Initialise OpenGL
    if (SDL_GL_LoadLibrary(NULL) != 0)
    {
        std::string error = SDL_GetError();
        ECHO_ERROR("Failed to open GL Library: {}", error);
        return -1;
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
    ECHO_WINDOWING::Window window("Test Window", 640, 480,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true,
        SDL_WINDOW_OPENGL);

    if (!window.GetWindow())
    {
        ECHO_ERROR("Failed to create window");
        return -1;
    }

    // Create OpenGL context
    window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));

    if (!window.GetGLContext())
    {
        std::string error = SDL_GetError();
        ECHO_ERROR("Failed to create OpenGL Context: {}", error);
        return -1;
    }

    SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
    SDL_GL_SetSwapInterval(1);

    // Initialise GLAD
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
    {
        ECHO_ERROR("Failed to LoadGL --> GLAD");
        return -1;
    }

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Add temp texture
    auto texture = ECHO_RENDERING::TextureLoader::Create(
        ECHO_RENDERING::Texture::TextureType::PIXEL,
        "./assets/textures/hill_tiles.png");

    if (!texture)
    {
        ECHO_ERROR("Failed to create texture");
        return -1;
    }

    // Temp UVs
    UVs tex_uvs;

    auto generate_uvs = [&](float start_x, float start_y, float sprite_width,
        float sprite_height)
        {
            tex_uvs.Width = sprite_width / texture->GetWidth();
            tex_uvs.Height = sprite_height / texture->GetHeight();

            tex_uvs.X = start_x * tex_uvs.Width;
            tex_uvs.Y = start_y * tex_uvs.Height;
        };

    generate_uvs(1, 6, 16, 16);

    std::vector<ECHO_RENDERING::Vertex> vertices{};

    ECHO_RENDERING::Vertex top_left{
        glm::vec2{-16.f, 16.f},
        glm::vec2{tex_uvs.X, (tex_uvs.Y + tex_uvs.Height)}
    };

    ECHO_RENDERING::Vertex top_right{
        glm::vec2{-16.f, -16.f},
        glm::vec2{tex_uvs.X, tex_uvs.Y}
    };

    ECHO_RENDERING::Vertex bot_left{
        glm::vec2{16.f, -16.f},
        glm::vec2{(tex_uvs.X + tex_uvs.Width), tex_uvs.Y}
    };

    ECHO_RENDERING::Vertex bot_right{
        glm::vec2{16.f, 16.f},
        glm::vec2{(tex_uvs.X + tex_uvs.Width), (tex_uvs.Y + tex_uvs.Height)}
    };

    vertices.push_back(top_left);
    vertices.push_back(top_right);
    vertices.push_back(bot_left);
    vertices.push_back(bot_right);

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create temp camera
    ECHO_RENDERING::Camera2D camera{};
    camera.SetScale(5.f);

    // Create shader
    auto shader = ECHO_RENDERING::ShaderLoader::Create(
        "assets/shaders/basic_shader");

    if (!shader)
    {
        ECHO_ERROR("Failed to create shader");
        return -1;
    }

    // Create vertex array object and vertex buffer object, index buffer object
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

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

    SDL_Event event{};
    bool running = true;

    // Window loop
    while (running)
    {
        // Process events
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

        glViewport(0, 0, window.GetWidth(), window.GetHeight());

        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Enable();
        glBindVertexArray(VAO);

        shader->SetUniformMat4("uProjection", camera.GetCameraMatrix());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        SDL_GL_SwapWindow(window.GetWindow().get());

        camera.Update();
        shader->Disable();
    }

    std::cout << "Closing\n";
    return 0;
}
