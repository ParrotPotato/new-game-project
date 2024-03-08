#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <GL/glew.h>

#include <unordered_map>

struct Texture2D{
    unsigned int width, height;
    unsigned int channel_count;
    GLuint id;
};
bool check_texture_equality(Texture2D t1, Texture2D t2);
Texture2D load_texture_2d(const char * texture_file_path, bool flip_uv_coordinate);
int unload_texture_2d(Texture2D * texture);

// struct Texture3D{};

struct TextureResourceHandler {
    std::unordered_map<std::string, Texture2D> textures_2d;
    // std::unordered_map<std::string, Texture3D> textures_3d;
};
Texture2D texture_resource_load_and_cache_texture_2d(TextureResourceHandler * resource_handler, const char * texture_file_path, bool flip_uv_coordinate);
int texture_resource_unload_texture_2d(TextureResourceHandler * resource_handler, const char * texture_file_path);

#endif
