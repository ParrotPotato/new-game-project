#include "shader.hh"

#include <GL/glew.h>

#include <cstdlib>
#include <cstdio>


// NOTE(nitesh): openGL calls in this file can be error checked using 
// the #define MACRO or using error message callback reporter

GLuint compile_shader(const char * source, GLenum type){
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success){
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char * error = (char *) malloc(length + 1);
        glGetShaderInfoLog(id, length, NULL, error);
        error[length] = 0;
        printf("Error occurred while compiling shader:\n%s\n", error);
        glDeleteShader(id);
        free(error);
        id = 0;
    }
    return id;
}

GLuint link_and_validate_program(GLuint shader_ids[], unsigned int size){
    GLuint id = glCreateProgram();
    for(unsigned int i = 0 ; i < size ; i++) {
        glAttachShader(id, shader_ids[i]);
    }
    glLinkProgram(id);

    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (id && !success) {
        GLint length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

        char * error = (char *) malloc(length + 1);
        glGetProgramInfoLog(id, length, NULL, error);
        error[length] = 0;
        printf("Error occurred while linking program:\n%s\n", error);
        glDeleteProgram(id);
        free(error);
        id = 0;
    }

    glGetProgramiv(id, GL_VALIDATE_STATUS, &success);
    if (id && !success) {
        GLint length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

        char * error = (char *) malloc(length + 1);
        glGetProgramInfoLog(id, length, NULL, error);
        error[length] = 0;
        printf("Error occurred while validating program:\n%s\n", error);
        glDeleteProgram(id);
        free(error);
        id = 0;
    }

    return id;
}
