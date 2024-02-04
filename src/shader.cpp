#include "shader.h"

#include "utils.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <utility>

using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::optional;
using std::nullopt;

namespace shader {

    std::optional<ShaderUnit> create_and_compile_shader_unit(const char * shader_source, GLenum shader_type){
        GLuint id = glCreateShader(shader_type);
        glShaderSource(id, 1, &shader_source, NULL);
        glCompileShader(id);

        GLint success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLint logLength;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

            char *logMessage = new char[logLength];
            glGetShaderInfoLog(id, logLength, NULL, logMessage);

            cerr << "Shader compilation failed:\n" << logMessage << endl;

            delete[] logMessage;
            glDeleteShader(id);

            return std::nullopt;
        }
        
        ShaderUnit result = {};
        result.id = id;
        result.type = shader_type;

        return result; 
    }

    // maybe we can later specify which sources loads what
    optional<ShaderProgram> create_shader_program(vector<std::pair<string, GLenum>> shader_source){

        optional<ShaderProgram> result;

        GLuint program = glCreateProgram();
        vector<ShaderUnit> compiled_unit_cache;

        // loading and compiling each component of the shader program
        for(const auto & shader_unit : shader_source) {
            string source_path = shader_unit.first;
            GLenum type   = shader_unit.second;

            optional<string> opt_source = utils::load_file_buffer(source_path.c_str());
            if (!opt_source) {
                cerr << "Failed to load shader unit file : " << source_path << endl;
                return nullopt;
            }
            auto shader_source = opt_source.value();

            optional<ShaderUnit> opt_compiled_unit = create_and_compile_shader_unit(shader_source.c_str(), type);
            if(!opt_compiled_unit) {
                cerr << "Failed to compile shader unit read from : " << source_path << endl;
                // Clearing generated shader cache
                for(const auto & compile_unit : compiled_unit_cache){
                    glDeleteShader(compile_unit.id);
                }
                return std::nullopt;
            }
            ShaderUnit compiled_unit = opt_compiled_unit.value();
            compiled_unit_cache.push_back(compiled_unit);
        }

        for(const auto & compiled_unit : compiled_unit_cache){
            glAttachShader(program, compiled_unit.id);
        }

        glLinkProgram(program);

        for(const auto & cu: compiled_unit_cache) {
            glDetachShader(program, cu.id);
            glDeleteShader(cu.id);
        }

        // Checking program validity
        GLint status;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
        if (!status) {
            GLchar infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
            std::cerr << "Shader program validation failed:\n" << infoLog << std::endl;
            glDeleteProgram(program);
            return nullopt;
        }
        
        ShaderProgram shader_program;
        shader_program.id = program;

        return shader_program;
    }
}
