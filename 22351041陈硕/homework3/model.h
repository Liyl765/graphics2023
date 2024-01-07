#pragma once

#include "shader.h"
#include "mesh.h"
//libs
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STBI_WINDOWS_UTF8

//std
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

inline uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
    //ģ������  
    std::vector<Texture> textures_loaded;
    std::vector<Mesh>    meshes;
    std::string          directory;
    bool gammaCorrection;
    glm::mat4 position;

    //��������
    Model(std::string const& path, bool gamma = false);

    //����ģ��ÿ��mesh
    void Draw(Shader shader);
private:
    //��ȡģ��
    void loadModel(std::string path);
    //����������
    void processNode(aiNode* node, const aiScene* scene);
    //��������
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    //���ز�������
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

