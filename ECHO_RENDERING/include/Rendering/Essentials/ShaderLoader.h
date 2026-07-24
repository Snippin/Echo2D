#pragma once

#include "Shader.h"

#include <glad/glad.h>
#include <string>

#include <memory>

namespace ECHO_RENDERING
{
    class ShaderLoader
    {
    public:
        ShaderLoader() = delete;

        static std::shared_ptr<Shader> Create(const std::string &base_path);

    private:
        static GLuint CreateProgram(const std::string &vertex_shader,
            const std::string &fragment_shader);
        static GLuint CompileShader(GLuint shader_type,
            const std::string &file_path);
        static bool CompileSuccess(GLuint shader);
        static bool IsProgramValid(GLuint program);
        static bool LinkShaders(GLuint program, GLuint vertex_shader,
            GLuint fragment_shader);
    };
}
