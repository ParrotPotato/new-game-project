#include "texture.hh"

#include <cstdio>

#include "deps/stb_image.h"

Texture2D load_texture_2d(const char * texture_file_path, bool flip_uv_coordinate) {
    Texture2D result;

    if (flip_uv_coordinate) {
        stbi_set_flip_vertically_on_load(true);
    } else {
        stbi_set_flip_vertically_on_load(false);
    }


    // Reading texture from file

    int width, height, channel_count;
    unsigned char * data = stbi_load(texture_file_path, &width, &height,  &channel_count, 0);
    if (data == nullptr) {
        printf("failed to load texture : %s\n", texture_file_path);
        printf("%s\n", stbi_failure_reason());
        return {};
    }

    // Creating and loading data in opengl texture

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Setting texture attribute
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (channel_count == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (channel_count == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    result.channel_count = channel_count;
    result.id = texture;

    return result;
}
