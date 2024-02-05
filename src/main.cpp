
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

namespace player{
}


int main(){

    std::printf("Starting ....\n");
    
    core::create_window_instance("main_window", 400, 300);

    std::pair<std::string, GLenum> vertex_shader_pair = make_pair(
        std::string("resource/shader/simple_vert_shader.glsl"), GL_VERTEX_SHADER);
    std::pair<std::string, GLenum> fragment_shader_pair = make_pair(
        std::string("resource/shader/simple_frag_shader.glsl"), GL_FRAGMENT_SHADER);
    

    std::optional<shader::ShaderProgram> program = shader::create_shader_program(
        vector<std::pair<std::string, GLenum>>{vertex_shader_pair, fragment_shader_pair});
    
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
        glUseProgram(program.value().id);
        printf("binding shader program with id: %d\n", program.value().id);
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
        printf("unbinding shader program with id: %d\n", program.value().id);
    }

    printf("Exiting  ....\n");

    return 0;
}