//
// Created by Niccolo on 30/03/2025.
//

#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture>textures);
    void render(const Shader &shader) const;
    auto getHeight(float x, float z) const -> float;
private:
    unsigned int VBO, EBO;
    void setupMesh();
};

#endif //MESH_H
