#ifndef MODEL_HH
#define MODEL_HH

#include <GL/glew.h>

enum Buffers{
    B_VERT,
    B_TEX,
    B_INDX,
    B_COUNT
};

struct Model {
    GLuint vao;
    GLuint buf[B_COUNT];
    unsigned int index_count;
};

Model load_entire_model(const char * file_source);

#endif
