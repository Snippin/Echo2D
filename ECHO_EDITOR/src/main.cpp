#define SDL_MAIN_HANDLED = 1;

#include <Windowing/Window/Window.h>
#include <SDL.h>
#include <glad/glad.h>

#include <iostream>

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);

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

	// Create temp vertex data
	//float vertices[] = {
	//	0.f, 0.5f, 0.f,
	//	-0.5f, -0.5f, 0.f,
	//	0.5f, -0.5f, 0.f,
	//};

	// Vertices for a quad
	float vertices[] = {
		// First triangle
		-0.5f, 0.5f, 0.f,  // TL
		0.5f, 0.5f, 0.f,   // TR
		0.5f, -0.5f, 0.f,  // BR
		-0.5f, -0.5f, 0.f, // BL
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	const char *vertex_src =
		"#version 450 core\n"
		"layout (location = 0) in vec3 aPosition;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPosition, 1.0f);\n"
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
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
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
		0,					// Attribute 0 -- Layout position in the shader
		3,					// Size		   -- Number of components per vertex
		GL_FLOAT,			// Type		   -- Data type of components
		GL_FALSE,			// Normalized  -- Specifies if fixed-point data values should be normalized
		3 * sizeof(float),	// Stride      -- Specifies byte offset between consecutive attributes
		(void *)0			// Pointer     -- Specifies the offset of the first component
	);

	glEnableVertexAttribArray(0);
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

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		SDL_GL_SwapWindow(window.GetWindow().get());
	}

	std::cout << "Closing\n";
	return 0;
}
