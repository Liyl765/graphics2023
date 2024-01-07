#pragma once
#include "shader.h"
//lib
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//std
#include <string>
#include <vector> 
#define MAX_BONE_INFLUENCE 4


struct Vertex {
	//λ��
	glm::vec3 Position;
	//������
	glm::vec3 Normal;
	//��������
	glm::vec2 TexCoords;
	//����
	glm::vec3 Tangent;
	//bitangent
	glm::vec3 Bitangent;
	//��Ӱ�쵽����Ĺ������
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//����Ӱ���Ȩ��
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	uint32_t id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	//��������
	std::vector<Vertex>           vertices;
	std::vector<uint32_t>         indices;
	std::vector<Texture>          textures;
	uint32_t VAO;

	//���캯��
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

	//��Ⱦ
	void Draw(Shader& shader);

private:
	uint32_t VBO, EBO;
	void setupMesh();
};