#include "model.hh"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <cstdio>

// NOTE(nitesh): Today we are going to work on model load and rendering aspect of our code
//
// 1. We might have to flip the uv coordinate system multiple times to get the right output

Model load_entire_model(const char * file_source){
    Assimp::Importer importer;
    const aiScene * model_scene = importer.ReadFile(file_source, aiProcess_Triangulate);
    if (model_scene == nullptr) {
        printf("error occurred while loading file : %s\n", file_source);
        printf("%s\n", importer.GetErrorString());
    }

    auto material = model_scene->mMaterials[0];

    // NOTE(nitesh): Loading mesh into our custom data starts

    printf("read mesh count : %d\n", model_scene->mNumMeshes);

    unsigned int vertex_count = 0;
    unsigned int tex_count    = 0;
    unsigned int index_count  = 0;

    for(unsigned int i = 0; i < model_scene->mNumMeshes; i++){
        auto mesh = model_scene->mMeshes[i];
        printf("mesh [%u] vertex count : %u\n", i, mesh->mNumVertices);
        printf("material index: %u\n", mesh->mMaterialIndex);

        // what is pIndex :: passing 0 for the time being
        if (mesh->HasTextureCoords(0)) {
            printf("index [%d] has texture coords\n", 0);
        }

        if (mesh->HasNormals()) {
            printf("mesh has normals\n");
        }

        vertex_count += mesh->mNumVertices * 3;
        tex_count += mesh->mNumVertices *2;
        index_count += mesh->mNumFaces  * 3;  // * 3 because we are using aiProcess_Triangulation
    }

    {
        printf("vertex count : %u\n", vertex_count);
        printf("tex count    : %u\n", tex_count);
        printf("index count  : %u\n", index_count);
    }

    // NOTE(nitesh): Allocating array size based on the num vertices and num faces

    float * vertices = (float *) malloc(sizeof(float) * vertex_count);
    unsigned int * indices  = (unsigned int *) malloc(sizeof(unsigned int) * index_count);
    float * tex_coordinates = (float *) malloc(sizeof(float) * tex_count);

    unsigned int voffset = 0; 
    unsigned int ioffset = 0;
    unsigned int toffset = 0;
    for(unsigned int i = 0 ; i < model_scene->mNumMeshes; i++){
        auto mesh = model_scene->mMeshes[i];
        for(unsigned int vi = 0 ; vi < mesh->mNumVertices ; vi++){
            vertices[voffset + 0] = mesh->mVertices[vi].x;
            vertices[voffset + 1] = mesh->mVertices[vi].y;
            vertices[voffset + 2] = mesh->mVertices[vi].z;
            
            voffset += 3;
        }
        // NOTE(nitesh): as per the spec if HasTexCoords is true then
        // the number of texture coordinates are equal as vertices
        for(unsigned int ti = 0 ; ti < mesh->mNumVertices ; ti++){
            // NOTE(nitesh): This is not entirely clear for me as to how they are 
            // organizing this structure
            tex_coordinates[toffset + 0] = mesh->mTextureCoords[0][ti].x;
            tex_coordinates[toffset + 1] = mesh->mTextureCoords[0][ti].y;
            toffset += 2;
        }

        for(unsigned int ii = 0; ii < mesh->mNumFaces ; ii++){

            if (mesh->mFaces[ii].mNumIndices != 3) {
                printf("Skipping face: more than 3 indices , please check triangulation\n");
                continue;
            }

            indices[ioffset + 0] = mesh->mFaces[ii].mIndices[0];
            indices[ioffset + 1] = mesh->mFaces[ii].mIndices[1];
            indices[ioffset + 2] = mesh->mFaces[ii].mIndices[2];

            ioffset += 3;
        }
    }

    GLuint vao;
    glGenVertexArrays(1 ,&vao);

    GLuint vbo, ibo, tbo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count, (void *) vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tex_count, (void *) tex_coordinates, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, (void *) indices, GL_STATIC_DRAW);

    // Free up the allocated memory
    free(vertices);
    free(indices);
    free(tex_coordinates);

    // Setting up vertex array

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Model result = {};
    result.vao = vao;
    result.buf[B_VERT] = vbo;
    result.buf[B_TEX]  = tbo;
    result.buf[B_INDX] = ibo;
    result.index_count = index_count;

    return result;
}


