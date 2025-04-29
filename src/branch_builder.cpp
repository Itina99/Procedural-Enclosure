//
// Created by Niccolo on 29/04/2025.
//

#include "branch_builder.h"

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

#include "utils.h"

Branch::Branch(unsigned int resolution): resolution(resolution) {
    this->tID = loadTexture("../textures/trees/Bark_007_SD/Bark_007_BaseColor.jpg");
}

void Branch::build_branch(float height, float R, float r) {
    std::vector<Vertex> vertices;

    double increment = glm::radians(360.0f/(float)this->resolution);

    float texture_sector = 1.0f/(float)this->resolution;
    // Generating sides of Truncated cone
    for (int i = 0; i < this->resolution; i++) {
        glm::vec3 v1(R * cos(i * increment), 0.0f, R * sin(i * increment));
        glm::vec3 v2(R * cos((i+1) * increment), 0.0f, R * sin((i+1) * increment));
        glm::vec3 v3(r * cos(i * increment), height, r * sin(i * increment));
        glm::vec3 v4(r * cos((i+1) * increment), height, r * sin((i+1) * increment));

        glm::vec3 face_vectorA(v3 - v1), face_vectorB(v2 - v1);
        glm::vec3 face_normal(glm::normalize(glm::cross(face_vectorA, face_vectorB)));
        glm::vec3 base_vectorA(v1 - glm::vec3(0.0)), base_vectorB(v2 - glm::vec3(0.0));
        glm::vec3 base_normal_down(glm::normalize(glm::cross(base_vectorA, base_vectorB))), base_normal_up(-base_normal_down);

        vertices.push_back(Vertex{.position=v1, .normal=face_normal, .texCoords=glm::vec2(i * texture_sector, 0)});
        vertices.push_back(Vertex{.position=v3, .normal=face_normal, .texCoords=glm::vec2(i * texture_sector, 1)});
        vertices.push_back(Vertex{.position=v2, .normal=face_normal, .texCoords=glm::vec2((i+1) * texture_sector, 0)});

        vertices.push_back(Vertex{.position=v3, .normal=face_normal, .texCoords=glm::vec2(i * texture_sector, 1)});
        vertices.push_back(Vertex{.position=v4, .normal=face_normal, .texCoords=glm::vec2((i+1) * texture_sector, 1)});
        vertices.push_back(Vertex{.position=v2, .normal=face_normal, .texCoords=glm::vec2((i+1) * texture_sector, 0)});

        vertices.push_back(Vertex{.position=glm::vec3(0, 0, 0), .normal=base_normal_down, .texCoords=glm::vec2(0, 0)});
        vertices.push_back(Vertex{.position=v1, .normal=base_normal_down, .texCoords=glm::vec2(0, 0)});
        vertices.push_back(Vertex{.position=v2, .normal=base_normal_down, .texCoords=glm::vec2(0, 0)});

        vertices.push_back(Vertex{.position=glm::vec3(0, height, 0), .normal=base_normal_up, .texCoords=glm::vec2(0, 0)});
        vertices.push_back(Vertex{.position=v4, .normal=base_normal_up, .texCoords=glm::vec2(0, 0)});
        vertices.push_back(Vertex{.position=v3, .normal=base_normal_up, .texCoords=glm::vec2(0, 0)});
    }

    std::vector<Texture> textures;
    textures.push_back(Texture{.id = this->tID, .type = ""});

    this->mesh = Mesh(vertices, std::vector<unsigned int>{}, textures);
}

Mesh Branch::getResult() {
    return this->mesh;
}