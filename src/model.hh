#ifndef MODEL_HH
#define MODEL_HH

#include <GL/glew.h>

#include <vector>

enum Buffers{
    B_VERT,
    B_TEX,
    B_INDX,
    B_COUNT
};

struct MeshInfo {
    unsigned int vertex_offset;
    unsigned int index_offset;
    unsigned int index_count;
};

struct Model {
    GLuint vao;
    GLuint buf[B_COUNT];
    unsigned int index_count;

    std::vector<MeshInfo> meshes;
};

Model load_entire_model(const char * file_source);

#endif
