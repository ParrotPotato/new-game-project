#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "core.hh"
#include "utils.hh"
#include "shader.hh"
#include "model.hh"
#include "texture.hh"

#include <cstdio>
#include <string>
#include <vector>

using std::string;
using std::vector;

// NOTE(nitesh): add colors to the debug output for clarity and easy spotting failures

// NOTE(nitesh): implementing a camera class
enum class CameraType {
    D2,
    D3,
};
struct Camera {
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::mat4 view;
};
Camera setup_camera(glm::vec3 position, glm::vec3 target){
    Camera result;
    result.target   = target;
    result.up       = glm::vec3(0.0, 1.0, 0.0);
    result.position = position;
    result.view     = glm::lookAt(result.position, result.target, result.up);
    return result;
}


glm::mat4 setup_orthographic_projection(float window_width, float window_height, float near_plane, float far_plane){
    glm::mat4 result= glm::ortho(0.0f, window_width, 0.0f, window_height, near_plane, far_plane);
    return result;
}
glm::mat4 setup_perspective_projection(float fov, float width, float height, float near_plane, float far_plane){
    glm::mat4 result= glm::perspective(glm::radians(fov), width / height, near_plane, far_plane);
    return result;
}


// NOTE(nitesh): Since this logic is going to be repeated
GLuint basic_shader_program(){
    //std::string vertex_source = read_and_load_file_in_buffer("resource/shader/simple_vert_shader.glsl");
    std::string vertex_source = read_and_load_file_in_buffer("resource/shader/model_rendering_vert.glsl");
    GLuint vertex_shader = compile_shader(vertex_source.c_str(), GL_VERTEX_SHADER);
    if (!vertex_shader){
        printf("failed to compile shader source : resource/shader/model_rendering_vert.glsl");
        return 0;
    }
    //std::string fragment_source= read_and_load_file_in_buffer("resource/shader/simple_frag_shader.glsl");
    std::string fragment_source= read_and_load_file_in_buffer("resource/shader/model_rendering_frag.glsl");
    GLuint fragment_shader = compile_shader(fragment_source.c_str(), GL_FRAGMENT_SHADER);
    if (!fragment_shader){
        printf("failed to compile shader source : resource/shader/model_rendering_frag.glsl");
        return 0;
    }
    GLuint shaders[] = {vertex_shader, fragment_shader};
    GLuint program = link_and_validate_program(shaders, 2);
    if (!program) {
        printf("failed to link and validate program source for basic_shader_program()");
        return 0;
    }
    return program;
}

GLuint * test_texture_rendering() {
    std::vector<GLfloat> vertices = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
    };

    // NOTE(nitesh): Colors for the triangle
    std::vector<GLfloat> tex =  {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);

    // buffer for vertices of the triangle
    GLuint verticesVBO;
    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // buffer for colors of the triangle
    GLuint texVBO;
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * tex.size(), tex.data(), GL_STATIC_DRAW);

    GLuint indicesVBO ;
    glGenBuffers(1, &indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint * result = (GLuint *) malloc(sizeof(GLuint) * 2);
    result[0] = vao;
    result[1] = indicesVBO;
    return result;
}

void original_testing_code(){
    // NOTE(nitesh): Vertices of a right triangle at 0.0
    std::vector<GLfloat> vertices = {
        0.0, 0.0, -500.0,
        0.0, 50.0, -500.0,
        50.0, 0.0, -500.0,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(1);
}

// NOTE(nitesh): Next task, setup program cameras which can move around the screen
int main(){

    std::printf("Starting ....\n");
    
    core::create_window_instance("main_window", 800, 600);

    GLuint program = basic_shader_program();
    

    if (!program) {
        printf("error occurred while generating shader program");
    }
    if (program) {
        glUseProgram(program);
        printf("binding shader program with id: %d\n", program);
    }

    Camera main_camera = setup_camera(glm::vec3(5, 5, 5), glm::vec3(0.0, 0.0, 0.0));

    glm::mat4 projection = setup_perspective_projection(45.0, 800, 600, 0.1, 1000.0f);
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = main_camera.view;


    printf("Test loading model start ---\n");
    Model new_model = load_entire_model("resource/models/kenney_mini-arena/Models/GLB format/banner.glb");
    printf("Test loading mode end ---\n");

    printf("Test loading texture start ---\n");
    Texture2D texture = load_texture_2d("resource/models/kenney_mini-arena/Models/GLB format/Textures/colormap.png", true);
    printf("Texture id :  %u\n", texture.id);
    printf("Test loading texture end ---\n");


    printf(" -- vertex array  : %d\n", new_model.vao);
    printf(" -- vertex buffer : %d\n", new_model.buf[B_VERT]);
    printf(" -- tex buffer    : %d\n", new_model.buf[B_TEX]);
    printf(" -- index buffer  : %d\n", new_model.buf[B_INDX]);

    /*
    glm::mat4 projection = glm::mat4(1.0);
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    */

    if (program) {
        glUseProgram(program);
        auto projectionUniform = glGetUniformLocation(program, "projection");
        auto modelUniform = glGetUniformLocation(program, "model");
        auto viewUniform = glGetUniformLocation(program, "view");

        auto texture0 = glGetUniformLocation(program, "colormap");

        printf("projection Matrix Uniform : %d\n", projectionUniform);
        printf("model Matrix Uniform      : %d\n", modelUniform);
        printf("view Matrix Uniform       : %d\n", viewUniform);
        printf("color map                 : %d\n", texture0);

        printf("Binding project matrix with value :\n%s\n", glm::to_string(projection).c_str());

        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniform1i(texture0, 0);
        glUseProgram(0);
    } 

    while (core::window_close_requested() == false) {
        core::clear_window();
        core::process_input();

        glUseProgram(program);

        // bind the correct texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id);


        glBindVertexArray(new_model.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_model.buf[B_INDX]);
        glDrawElements(GL_TRIANGLES, new_model.index_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        if(core::key_down(SDLK_ESCAPE)) {
            break;
        }
        
        core::update_window();
    }

    if (program){
        glUseProgram(0);
        printf("unbinding shader program with id: %d\n", program);
    }

    printf("Exiting  ....\n");

    return 0;
}
