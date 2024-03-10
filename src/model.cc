#include "model.hh"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <cstdio>

// NOTE(nitesh): Today we are going to work on model load and rendering aspect of our code
//
// 1. We might have to flip the uv coordinate system multiple times to get the right output
// 2. Not using normal values for the time being

Model load_entire_model(const char * file_source, TextureResourceHandler * rh){
    Assimp::Importer importer;
    const aiScene * model_scene = importer.ReadFile(file_source, aiProcess_Triangulate);
    if (model_scene == nullptr) {
        printf("error occurred while loading file : %s\n", file_source);
        printf("%s\n", importer.GetErrorString());
    }

    auto material = model_scene->mMaterials[0];

    // NOTE(nitesh): Loading mesh into our custom data starts

    printf("read mesh count : %d\n", model_scene->mNumMeshes);

    unsigned int total_vertex_count = 0;
    unsigned int total_normal_count = 0;
    unsigned int total_tex_count    = 0;
    unsigned int total_index_count  = 0;

    std::vector<Texture2D> textures;
    std::vector<MeshInfo> meshes;

    { // scope for loading all the textures, can be made a function if we are using this again
        std::string::size_type name_offset = std::string(file_source).find_last_of("/");
        std::string dir; 
        if (name_offset == std::string::npos) {
            dir = ".";
        } else if (name_offset == 0) {
            dir = "/";
        } else {
            dir = std::string(file_source).substr(0, name_offset);
        }

        for(unsigned int i = 0 ; i < model_scene->mNumMaterials ; i++){
            const aiMaterial * material = model_scene->mMaterials[i];

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
                aiString texture_path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
                    std::string p(texture_path.data);

                    if (p.substr(0, 2) == ".\\") {
                        p = p.substr(2, p.size() - 2);
                    }
                    std::string full_path = dir + "/" + p;
                    // NOTE(nitesh): Check if we need to change the texture loading or can we just leave this true 
                    // here
                    Texture2D texture = texture_resource_load_and_cache_texture_2d(rh, full_path.c_str(), true);

                    if (texture.id == 0){
                        printf("failed to load texture for model: %s\n -- texture file path : %s\n", file_source, full_path.c_str());
                    }
                    textures.push_back(texture);
                }
            }
        }
    }

    for(unsigned int i = 0; i < model_scene->mNumMeshes; i++){
        aiMesh * mesh = model_scene->mMeshes[i];
        printf("mesh [%u] vertex count : %u\n", i, mesh->mNumVertices);
        printf("material index: %u\n", mesh->mMaterialIndex);

        // what is pIndex :: passing 0 for the time being
        if (mesh->HasTextureCoords(0)) {
            printf("index [%d] has texture coords\n", 0);
        }

        if (mesh->HasNormals()) {
            printf("mesh has normals\n");
        }

        MeshInfo mesh_info;
        mesh_info.vertex_offset = total_vertex_count;
        mesh_info.index_offset  = total_index_count;
        mesh_info.index_count   = mesh->mNumFaces * 3;
        mesh_info.texture_index = mesh->mMaterialIndex;
        mesh_info.name          = std::string(mesh->mName.data);

        total_vertex_count += mesh->mNumVertices;
        total_normal_count += mesh->mNumVertices;
        total_tex_count    += mesh->mNumVertices;
        total_index_count  += mesh->mNumFaces  * 3;  // * 3 because we are using aiProcess_Triangulation

        meshes.push_back(mesh_info);
    }
    
    // NOTE(nitesh): Allocating array size based on the num vertices and num faces

    float * vertices = (float *) malloc(sizeof(float) * total_vertex_count * 3);
    float * normals  = (float *) malloc(sizeof(float) * total_normal_count * 3);
    unsigned int * indices  = (unsigned int *) malloc(sizeof(unsigned int) * total_index_count);
    float * tex_coordinates = (float *) malloc(sizeof(float) * total_tex_count * 2);

    unsigned int voffset = 0; 
    unsigned int noffset = 0;
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

        for(unsigned int ni = 0 ; ni < mesh->mNumVertices ; ni++){
            normals[noffset + 0] = mesh->mNormals[ni].x;
            normals[noffset + 1] = mesh->mNormals[ni].y;
            normals[noffset + 2] = mesh->mNormals[ni].z;
            
            noffset += 3;
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

    GLuint vbo, nbo, ibo, tbo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * total_vertex_count * 3, (void *) vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * total_normal_count * 3, (void *) normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * total_tex_count * 2, (void *) tex_coordinates, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * total_index_count, (void *) indices, GL_STATIC_DRAW);

    // Free up the allocated memory
    free(vertices);
    free(normals);
    free(indices);
    free(tex_coordinates);

    // Setting up vertex array

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    Model result = {};
    result.vao = vao;
    result.buf[B_VERT] = vbo;
    result.buf[B_NORM] = nbo;
    result.buf[B_TEX]  = tbo;
    result.buf[B_INDX] = ibo;
    result.index_count = total_index_count;
    result.meshes = meshes;
    result.textures = textures;

    return result;
}



// NOTE(nitesh): please update both these functions when rendering logic changes
// 1. render entire model 
// 2. render single mesh of model 

void render_entire_model(const Model &model){
    glBindVertexArray(model.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.buf[B_INDX]);
    for(unsigned int i = 0; i < model.meshes.size() ; i++) {
        MeshInfo mesh_info = model.meshes[i];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.textures[mesh_info.texture_index].id);
        glDrawElementsBaseVertex(
                GL_TRIANGLES, 
                mesh_info.index_count, 
                GL_UNSIGNED_INT, 
                (void *) (sizeof(unsigned int) * mesh_info.index_offset), 
                mesh_info.vertex_offset);
    }
}
void render_single_mesh_of_model(const Model & model, unsigned long index){
    glBindVertexArray(model.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.buf[B_INDX]);
    if ( index < model.meshes.size()) {
        MeshInfo mesh_info = model.meshes[index];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.textures[mesh_info.texture_index].id);
        glDrawElementsBaseVertex(
                GL_TRIANGLES, 
                mesh_info.index_count, 
                GL_UNSIGNED_INT, 
                (void *) (sizeof(unsigned int) * mesh_info.index_offset), 
                mesh_info.vertex_offset);
    } else {
        printf("- mesh at index %lu does not exist\n- model has %lu meshes", index, model.meshes.size());
    }
}
