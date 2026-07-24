#include "Rendering/Essentials/Shader.h"

#include <Logger/Logger.h>

namespace ECHO_RENDERING
{
    Shader::Shader(GLuint program, const std::string &vertex_path,
        const std::string &fragment_path) :
        shader_program_id{program}, vertex_path{vertex_path},
        fragment_path{fragment_path}
    {
    }

    Shader::~Shader()
    {
        if (shader_program_id > 0)
        {
            glDeleteProgram(shader_program_id);
        }
    }

    void Shader::Enable()
    {
        glUseProgram(shader_program_id);
    }

    void Shader::Disable()
    {
        glUseProgram(0);
    }

    GLuint Shader::ShaderProgramID() const
    {
        return shader_program_id;
    }

    void Shader::SetUniformInt(const std::string &name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    GLuint Shader::GetUniformLocation(const std::string &uniform_name)
    {
        auto uniform_itr = uniform_location_map.find(uniform_name);

        if (uniform_itr != uniform_location_map.end())
        {
            return uniform_itr->second;
        }

        GLuint location = glGetUniformLocation(shader_program_id,
            uniform_name.c_str());

        if (location == GL_INVALID_INDEX)
        {
            ECHO_ERROR("Uniform [{}] is not found in the shader",
                uniform_name);
            return -1;
        }

        uniform_location_map.emplace(uniform_name, location);

        return location;
    }
}
