#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

#include <iostream>
#include <vector>
#include "shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(std::string path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader)
    {
        for (int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

