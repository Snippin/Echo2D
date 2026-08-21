#include "Rendering/Essentials/ShaderLoader.h"

#include <fstream>
#include <iostream>

namespace ECHO_RENDERING
{
    std::shared_ptr<Shader> ShaderLoader::Create(const std::string &base_path)
    {
        std::string vertex_shader_path = base_path + ".vert";
        std::string fragment_shader_path = base_path + ".frag";

        GLuint program = CreateProgram(vertex_shader_path,
            fragment_shader_path);

        if (program)
        {
            return std::make_shared<Shader>(program, vertex_shader_path,
                fragment_shader_path);
        }

        return nullptr;
    }

    GLuint ShaderLoader::CreateProgram(const std::string &vertex_shader,
        const std::string &fragment_shader)
    {
        const GLuint program = glCreateProgram();
        const GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertex_shader);
        const GLuint fragment = CompileShader(GL_FRAGMENT_SHADER,
            fragment_shader);

        if (vertex == 0 || fragment == 0)
        {
            return 0;
        }

        if (!LinkShaders(program, vertex, fragment))
        {
            std::cout << "Failed to link shaders\n";
            return 0;
        }

        return program;
    }

    GLuint ShaderLoader::CompileShader(GLuint shader_type,
        const std::string &file_path)
    {
        const GLuint shader_id = glCreateShader(shader_type);

        std::ifstream ifs(file_path);

        if (ifs.fail())
        {
            std::cout << "Shader failed to open [" << file_path << "]\n";
            return 0;
        }

        std::string contents;
        std::string line;

        while (std::getline(ifs, line))
        {
            contents += line + "\n";
        }

        ifs.close();

        const char *contents_ptr = contents.c_str();
        glShaderSource(shader_id, 1, &contents_ptr, nullptr);

        glCompileShader(shader_id);

        if (!CompileSuccess(shader_id))
        {
            std::cout << "Shader failed to compile [" << file_path << "]\n";
            return 0;
        }

        return shader_id;
    }

    bool ShaderLoader::CompileSuccess(GLuint shader)
    {
        GLint status;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint max_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

            std::string log(max_length, ' ');
            glGetShaderInfoLog(shader, max_length, &max_length, log.data());

            std::cout << "Shader compilation failed: " << std::string(log) <<
                "\n";

            glDeleteShader(shader);
            return false;
        }

        return true;
    }

    bool ShaderLoader::IsProgramValid(GLuint program)
    {
        GLint status;

        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint max_length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

            std::string log(max_length, ' ');
            glGetProgramInfoLog(program, max_length, &max_length, log.data());

            std::cout << "Shader program failed to link: " <<
                std::string(log) << "\n";
            return false;
        }

        return true;
    }

    bool ShaderLoader::LinkShaders(GLuint program, GLuint vertex_shader,
        GLuint fragment_shader)
    {
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);

        if (!IsProgramValid(program))
        {
            glDeleteProgram(program);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            return false;
        }

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return true;
    }
}
