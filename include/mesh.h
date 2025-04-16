//
// Created by Niccolo on 30/03/2025.
//

#ifndef MESH_H
#define MESH_H
#include <vector>
#include <glm/vec3.hpp>
#include <glad/glad.h>


struct Vertex {
    glm::vec3 position;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void render() const;
    auto getHeight(float x, float z) const -> float;
private:
    unsigned int VBO, EBO;
    void setupMesh();
};

#endif //MESH_H
