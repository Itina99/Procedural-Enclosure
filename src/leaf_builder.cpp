//
// Created by Niccolo on 29/04/2025.
//

#include "leaf_builder.h"

#include "utils.h"

Leaf::Leaf(float size) : size(size){
    this->tID = loadTexture("../textures/trees/Leaves.png");
}

void Leaf::build_leaf() {
    std::vector<Vertex> vertices;

    glm::vec3 v1 = glm::vec3(-this->size/2, 0.0f, 0.0f);
    glm::vec3 v2 = glm::vec3(this->size/2, 0.0f, 0.0f);
    glm::vec3 v3 = glm::vec3(-this->size/2, this->size, 0.0f);
    glm::vec3 v4 = glm::vec3(this->size/2, this->size, 0.0f);

    vertices.push_back(Vertex{.position=v1, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(0.0f, 0.06f)});
    vertices.push_back(Vertex{.position=v3, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(0.0f, 1.0f)});
    vertices.push_back(Vertex{.position=v2, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(1.0f, 0.06f)});

    vertices.push_back(Vertex{.position=v3, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(0.0f, 1.0f)});
    vertices.push_back(Vertex{.position=v4, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(1.0f, 1.0f)});
    vertices.push_back(Vertex{.position=v2, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(1.0f, 0.06f)});

    std::vector<Texture> textures;
    textures.push_back(Texture{.id = this->tID, .type = ""});

    this->mesh = Mesh(vertices, std::vector<unsigned int>{}, textures);
}

Mesh Leaf::getResult() {
    return this->mesh;
}
