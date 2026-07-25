#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace ECHO_RENDERING
{
    class Shader
    {
    public:
        Shader();
        Shader(GLuint program, const std::string &vertex_path,
            const std::string &fragment_path);
        ~Shader();

        void Enable();
        void Disable();

        GLuint ShaderProgramID() const;

        void SetUniformInt(const std::string &name, int value);
        void SetUniformMat4(const std::string &name, const glm::mat4 &mat);

        // TODO: Add more Getters & Setters as required

    private:
        GLuint shader_program_id;
        std::string vertex_path;
        std::string fragment_path;

        std::unordered_map<std::string, GLuint> uniform_location_map;

        GLuint GetUniformLocation(const std::string &uniform_name);
    };
}
