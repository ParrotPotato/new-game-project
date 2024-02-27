#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <GL/glew.h>

struct Texture2D{
    unsigned int channel_count;
    GLuint id;
};

Texture2D load_texture_2d(const char * texture_file_path, bool flip_uv_coordinate);

#endif
