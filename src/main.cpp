#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "core.h"
#include "utils.h"
#include "shader.h"

#include <cstdio>

#include <string>
#include <vector>

#include <optional>

using std::string;
using std::vector;

// NOTE(nitesh): Since this logic is going to be repeated
GLuint basic_shader_program(){
    std::string vertex_source = utils::load_file_buffer("resource/shader/simple_vert_shader.glsl").value();
    GLuint vertex_shader = shader::compile_shader(vertex_source.c_str(), GL_VERTEX_SHADER);
    if (!vertex_shader){
        printf("failed to compile shader source : resource/shader/simple_vert_shader.glsl");
        return 0;
    }
    std::string fragment_source= utils::load_file_buffer("resource/shader/simple_frag_shader.glsl").value();
    GLuint fragment_shader = shader::compile_shader(fragment_source.c_str(), GL_FRAGMENT_SHADER);
    if (!fragment_shader){
        printf("failed to compile shader source : resource/shader/simple_frag_shader.glsl");
        return 0;
    }
    GLuint shaders[] = {vertex_shader, fragment_shader};
    GLuint program = shader::link_and_validate_program(shaders, 2);
    if (!program) {
        printf("failed to link and validate program source for basic_shader_program()");
        return 0;
    }
    return program;
}

// NOTE(nitesh): Next task, setup program cameras which can move around the screen

int main(){

    std::printf("Starting ....\n");
    
    core::create_window_instance("main_window", 400, 300);

    GLuint program = basic_shader_program();
    
    // NOTE(nitesh): Vertices of a right triangle at 0.0
    std::vector<GLfloat> vertices = {
        0.0, 0.0, 
        0.0, 1.0,
        1.0, 0.0
    };

    // NOTE(nitesh): Colors for the triangle
    std::vector<GLfloat> colors =  {
        0.0, 0.0, 0.0, 
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
    };

    // NOTE(nitesh): Index for triangle rendering
    std::vector<GLuint> indices = {
        0, 1, 2
    };
    
    //vao 
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // buffer for vertices of the triangle
    GLuint verticesVBO;
    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // buffer for colors of the triangle
    GLuint colorsVBO;
    glGenBuffers(1, &colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), colors.data(), GL_STATIC_DRAW);

    GLuint indicesVBO ;
    glGenBuffers(1, &indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // setting up vertex attrib object for rendering
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(1);
    
    if (!program) {
        printf("error occurred while generating shader program");
    }
    if (program) {
        glUseProgram(program);
        printf("binding shader program with id: %d\n", program);
    }

    while (core::window_close_requested() == false) {
        core::clear_window();
        core::process_input();

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        core::update_window();
    }

    if (program){
        glUseProgram(0);
        printf("unbinding shader program with id: %d\n", program);
    }

    printf("Exiting  ....\n");

    return 0;
}
