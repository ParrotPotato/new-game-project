#include "texture.hh"

#include <cstdio>

#include "deps/stb_image.h"

const Texture2D UNINITIALISED_TEXTURE = {};

bool check_texture_2d_equality(Texture2D t1, Texture2D t2){
    return t1.width == t2.width && t1.height == t2.height && t1.channel_count == t2.channel_count && t1.id == t2.id;
}

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

    printf("texture id : %d loaded from %s\n", texture, texture_file_path);
    result.channel_count = channel_count;
    result.id = texture;
    result.width  = (unsigned int) width;
    result.height = (unsigned int) height;

    return result;
}

int unload_texture_2d(Texture2D * texture) {
    printf("unloading texture with id : %d\n", texture->id);
    glDeleteTextures(1, &texture->id);
    *texture = UNINITIALISED_TEXTURE;
    return 1;
}

Texture2D texture_resource_load_and_cache_texture_2d(TextureResourceHandler * resource_handler, const char * texture_file_path, bool flip_uv_coordinate){
    std::string path = texture_file_path;
    if (resource_handler->textures_2d.find(path) == resource_handler->textures_2d.end()){
        Texture2D texture = load_texture_2d(texture_file_path, flip_uv_coordinate);
        if (check_texture_2d_equality(texture, UNINITIALISED_TEXTURE)){
            return UNINITIALISED_TEXTURE;
        }
        resource_handler->textures_2d[path] = texture;
    }
    return resource_handler->textures_2d[path];
}

int texture_resource_unload_texture_2d(TextureResourceHandler * resource_handler, const char * texture_file_path){
    std::string path = texture_file_path;
    if (resource_handler->textures_2d.find(path) != resource_handler->textures_2d.end()){
        Texture2D texture = resource_handler->textures_2d[path];
        resource_handler->textures_2d.erase(path);
        unload_texture_2d(&texture);
        return 1;
    }
    return 0;
}
