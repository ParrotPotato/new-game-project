
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "core.h"
#include "shader.h"

#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

#include <optional>
#include <utility>

using std::string;
using std::pair;
using std::make_pair;
using std::vector;


int main(){

    printf("Starting ....\n");
    
    core::create_window_instance("main_window", 400, 300);

    std::pair<std::string, GLenum> vertex_shader_pair = make_pair(
        std::string("resource/shader/simple_vert_shader.glsl"), GL_VERTEX_SHADER);
    std::pair<std::string, GLenum> fragment_shader_pair = make_pair(
        std::string("resource/shader/simple_frag_shader.glsl"), GL_FRAGMENT_SHADER);
    

    std::optional<shader::ShaderProgram> program = shader::create_shader_program(
        vector<std::pair<std::string, GLenum>>{vertex_shader_pair, fragment_shader_pair});
    
    if (!program) {
        printf("error occurred while generating shader program");
    }

    if (program) {
        glUseProgram(program.value().id);
        printf("binding shader program with id: %d\n", program.value().id);
    }

    while (core::window_close_requested() == false) {
        core::clear_window();
        core::process_input();
        
        core::update_window();
    }

    if (program){
        glUseProgram(0);
        printf("unbinding shader program with id: %d\n", program.value().id);
    }

    printf("Exiting  ....\n");

    return 0;
}