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

    Camera main_camera = setup_camera(glm::vec3(0, 5, 5), glm::vec3(0.0, 0.0, 0.0));

    glm::mat4 projection = setup_perspective_projection(45.0, 800, 600, 0.1, 1000.0f);
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = main_camera.view;


    printf("Test loading model start ---\n");
    TextureResourceHandler rh = {};

    Model new_model = load_entire_model("resource/models/kenney_mini-arena/Models/GLB format/character-soldier.glb", &rh);

    printf("-- Mesh info start --");
    for(unsigned int i = 0 ; i < new_model.meshes.size() ; i++) {
        printf("mesh index : %u\n",i);
        printf("--   vertex offset : %u\n", new_model.meshes[i].vertex_offset);
        printf("--   index offset  : %u\n", new_model.meshes[i].index_offset);
        printf("--   index count   : %u\n", new_model.meshes[i].index_count);
    }
    printf("--  Mesh info end  --\n");

    printf("Test loading mode end ---\n");

    // NOTE(nitesh): setting up shader uniforms which will be used for rendering
    if (program) {
        glUseProgram(program);
        auto projectionUniform = glGetUniformLocation(program, "projection");
        auto modelUniform = glGetUniformLocation(program, "model");
        auto viewUniform = glGetUniformLocation(program, "view");
        auto texture0 = glGetUniformLocation(program, "colormap");
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniform1i(texture0, 0);
        glUseProgram(0);
    } 

    unsigned long mesh_model_index = 0;
    bool fullmodel = true;
    while (core::window_close_requested() == false) {
        core::clear_window();
        core::process_input();

        glUseProgram(program);

        if (fullmodel) { 
            render_entire_model(new_model);
        } else {
            render_single_mesh_of_model(new_model, mesh_model_index);
        }

        glUseProgram(0);
        
        if(core::key_down(SDLK_ESCAPE)) {
            break;
        }
        if(core::key_pressed(SDLK_f)){
            fullmodel = !fullmodel;
            if (fullmodel) {
                printf("Switched to full model rendering\n");
            } else {
                printf("Switched to parrt based rendering\n");
            }
        }
        if(core::key_pressed(SDLK_SPACE)){ 
            mesh_model_index = (mesh_model_index + 1) % new_model.meshes.size();
            printf("rendering model part :  %s\n", new_model.meshes[mesh_model_index].name.c_str());
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
