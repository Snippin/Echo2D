#define SDL_MAIN_HANDLED = 1;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <SOIL2/SOIL2.h>
#include <Windowing/Window/Window.h>

#include <iostream>

class Camera2D
{
public:
    Camera2D()
        : Camera2D(640, 480)
    {
    }

    Camera2D(int width, int height) :
        width{width}, height{height}, scale{1.f},
        position{glm::vec2{0.f}}, camera_matrix{1.f},
        ortho_projection{1.f}, need_update{true}
    {
        float half_width = static_cast<float>(width) * 0.5f;
        float half_height = static_cast<float>(height) * 0.5f;

        // Flip top and bottom values due to image being rendered upside down
        ortho_projection = glm::ortho(
            -half_width,    // Left
            half_width,     // Right
            half_height,    // Bottom
            -half_height,   // Top
            -1.f,           // Near
            1.f             // Far
        );
    }

    inline glm::mat4 GetCameraMatrix() { return camera_matrix; }

    inline void SetScale(float new_scale)
    {
        scale = new_scale;
        need_update = true;
    }

    void Update()
    {
        if (!need_update)
        {
            return;
        }

        // Translate
        glm::vec3 translate{-position.x, -position.y, 0.f};
        camera_matrix = glm::translate(ortho_projection, translate);

        // Scale
        glm::vec3 scaling{scale, scale, 0.f};
        camera_matrix *= glm::scale(glm::mat4{1.f}, scaling);

        need_update = false;
    }

private:
    int width;
    int height;
    float scale;

    glm::vec2 position;
    glm::mat4 camera_matrix;
    glm::mat4 ortho_projection;

    bool need_update;
};

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

bool LoadTexture(const std::string &path, int &width, int &height,
    bool blended)
{
    int channels = 0;

    unsigned char *image = SOIL_load_image(
        path.c_str(),   // File name      -- Image to be loaded
        &width,         // Width          -- Width of image
        &height,        // Height         -- Height of image
        &channels,      // Channels       -- Number of channels
        SOIL_LOAD_AUTO  // Force channels -- Force channel count
    );

    // Check if image is successful
    if (!image)
    {
        std::cout << "SOIL2 failed to load image [" << path << "] -- " <<
            SOIL_last_result() << "\n";
        return false;
    }

    GLint format = GL_RGBA;

    switch (channels)
    {
    case 3:
        format = GL_RGB;
        break;

    case 4:
        format = GL_RGBA;
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (blended)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    glTexImage2D(
        GL_TEXTURE_2D,      // Target           -- Specifies target texture
        0,                  // Level            -- Level of detail. 0 is base image level
        format,             // Internal format  -- Number of color components
        width,              // Width            -- Width of texture image
        height,             // Height           -- Height of texture image
        0,                  // Border
        format,             // Format           -- Format of pixel data
        GL_UNSIGNED_BYTE,   // Type             -- Data type of pixel data
        image               // Data
    );

    // Delete image data from SOIL2
    free(image);

    return true;
}

int main()
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::string error = SDL_GetError();
        std::cout << "Failed to intialise SDL: " << error << "\n";
        return -1;
    }

    // Initialise OpenGL
    if (SDL_GL_LoadLibrary(NULL) != 0)
    {
        std::string error = SDL_GetError();
        std::cout << "Failed to open GL Library: " << error << "\n";
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
        std::cout << "Failed to create window\n";
        return -1;
    }

    // Create OpenGL context
    window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));

    if (!window.GetGLContext())
    {
        std::string error = SDL_GetError();
        std::cout << "Failed to create OpenGL Context: " << error << "\n";
        return -1;
    }

    SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
    SDL_GL_SetSwapInterval(1);

    // Initialise GLAD
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
    {
        std::cout << "Failed to LoadGL --> GLAD\n";
        return -1;
    }

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create texture id & gen/bind texture
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    // Create width & height for texture
    int width = 0;
    int height = 0;

    // Load texture
    if (!LoadTexture("assets/textures/hill_tiles.png", width, height, false))
    {
        std::cout << "Failed to load the texture\n";
        return -1;
    }

    // Temp UVs
    UVs uvs;

    auto generate_uvs = [&](float start_x, float start_y, float sprite_width,
        float sprite_height)
        {
            uvs.Width = sprite_width / width;
            uvs.Height = sprite_height / height;

            uvs.X = start_x * uvs.Width;
            uvs.Y = start_y * uvs.Height;
        };

    generate_uvs(1, 6, 16, 16);

    // Vertices for a quad
    //float vertices[] = {
    //    -0.5f, 0.5f, 0.f, 0.f, 1.f,  // TL
    //    0.5f, 0.5f, 0.f, 1.f, 1.f,   // TR
    //    0.5f, -0.5f, 0.f, 1.f, 0.f,  // BR
    //    -0.5f, -0.5f, 0.f, 0.f, 0.f, // BL
    //};

    // Swapped tex coords
    float vertices[] = {
        -16, 16.f, 0.f, uvs.X, (uvs.Y + uvs.Height),                    // TL
        -16.f, -16.f, 0.f, uvs.X, uvs.Y,                                // BL
        16.f, -16.f, 0.f, (uvs.X + uvs.Width), uvs.Y,                   // BR
        16.f, 16.f, 0.f, (uvs.X + uvs.Width), (uvs.Y + uvs.Height)      // TR
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create temp camera
    Camera2D camera{};
    camera.SetScale(5.f);

    const char *vertex_src =
        "#version 450 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "uniform mat4 uProjection;\n"
        "out vec2 fragUVs;\n"
        "void main()\n"
        "{\n"
        "	gl_Position = uProjection * vec4(aPos, 1.0f);\n"
        "	fragUVs = aTexCoords;\n"
        "}\0";

    // Create the shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Add vertex shader source
    glShaderSource(vertex_shader, 1, &vertex_src, NULL);

    // Compile vertex shader
    glCompileShader(vertex_shader);

    // Get compilation status
    int status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        char log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, log);
        std::cout << "Failed to compile vertex shader\n" << log << "\n";
        return -1;
    }

    // Create temp fragment shader
    const char *fragment_src =
        "#version 450 core\n"
        "in vec2 fragUVs;\n"
        "out vec4 color;\n"
        "uniform sampler2D uTex;\n"
        "void main()\n"
        "{\n"
        //"	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
        "	color = texture(uTex, fragUVs);\n"
        "}\0";

    // Create the shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Add vertex shader source
    glShaderSource(fragment_shader, 1, &fragment_src, NULL);

    // Compile fragment shader
    glCompileShader(fragment_shader);

    // Get compilation status
    status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        char log[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, log);
        std::cout << "Failed to compile fragment shader\n" << log << "\n";
        return -1;
    }

    // Create shader program
    GLuint shader_program = glCreateProgram();

    // Attach shaders to program
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Link the program
    glLinkProgram(shader_program);

    // Check link status
    status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);

    if (!status)
    {
        char log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, log);
        std::cout << "Failed to link shader program\n" << log << "\n";
        return -1;
    }

    // Enable shader program
    glUseProgram(shader_program);

    // Delete shaders as they are linked
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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
        GL_ARRAY_BUFFER,					   // Target buffer type
        sizeof(vertices) * 3 * sizeof(float),  // size of buffer object data in bytes
        vertices,							   // Pointer to data that will be copied
        GL_STATIC_DRAW						   // Expected usage pattern of data
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,			   // Target buffer type
        6 * sizeof(GLuint),                    // size of buffer object data in bytes
        indices,							   // Pointer to data that will be copied
        GL_STATIC_DRAW						   // Expected usage pattern of data
    );

    glVertexAttribPointer(
        0,					// Attribute    -- Layout position in the shader
        3,					// Size		    -- Number of components per vertex
        GL_FLOAT,			// Type		    -- Data type of components
        GL_FALSE,			// Normalized   -- Specifies if fixed-point data values should be normalized
        5 * sizeof(float),	// Stride       -- Specifies byte offset between consecutive attributes
        nullptr 			// Pointer      -- Specifies the offset of the first component
    );

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        reinterpret_cast<void *>(sizeof(float) * 3) // Offset of positional data to the first UV coord
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
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

        glUseProgram(shader_program);
        glBindVertexArray(VAO);

        auto projection = camera.GetCameraMatrix();
        GLuint location = glGetUniformLocation(shader_program, "uProjection");

        glUniformMatrix4fv(location, 1, GL_FALSE, &projection[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_id);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        SDL_GL_SwapWindow(window.GetWindow().get());

        camera.Update();
    }

    std::cout << "Closing\n";
    return 0;
}
