#ifndef SHADER
#define SHADER

#include <GL/glew.h>

#include <optional>
#include <utility>
#include <string>
#include <vector>

namespace shader {
    struct ShaderUnit{GLuint id; GLenum type;};
    struct ShaderProgram{GLuint id;};

    std::optional<ShaderProgram> create_shader_program(std::vector<std::pair<std::string, GLenum>> shader_source);
}

#endif