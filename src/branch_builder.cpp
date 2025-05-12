//
// Created by Niccolo on 29/04/2025.
//

#include "branch_builder.h"

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

#include "utils.h"

Branch::Branch(const char* texture_path, unsigned int resolution): resolution(resolution) {
    this->tID = loadTexture(texture_path);
}

void Branch::build_branch(float height, float R, float r) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int startIndex;

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

         startIndex = static_cast<unsigned int>(vertices.size());

        // Posizionamento dei vertici delle facce laterali
        vertices.push_back(Vertex{.position=v1, .normal=face_normal, .texCoords=glm::vec2(i * texture_sector, 0)});
        vertices.push_back(Vertex{.position=v2, .normal=face_normal, .texCoords=glm::vec2((i+1) * texture_sector, 0)});

        vertices.push_back(Vertex{.position=v3, .normal=face_normal, .texCoords=glm::vec2(i * texture_sector, 1)});
        vertices.push_back(Vertex{.position=v4, .normal=face_normal, .texCoords=glm::vec2((i+1) * texture_sector, 1)});

        indices.push_back(startIndex);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 1);

        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 3);
        indices.push_back(startIndex + 1);
    }

    // Posizionamento dei vertici della base inferiore

    startIndex = static_cast<unsigned int>(vertices.size());
    vertices.push_back(Vertex{.position=glm::vec3(0, 0, 0), .normal=glm::vec3(0.0f, -1.0f, 0.0f), .texCoords=glm::vec2(0, 0)});
    unsigned int baseIndex = static_cast<unsigned int>(vertices.size());

    for (int i = 0; i < this->resolution; i++) {
        glm::vec3 v(R * cos(i * increment), 0.0f, R * sin(i * increment));
        vertices.push_back(Vertex{.position=v, .normal=glm::vec3(0.0f, -1.0f, 0.0f), .texCoords=glm::vec2(0, 0)});
    }
    for (int i = 0; i < this->resolution; i++) {
        indices.push_back(startIndex);
        indices.push_back(baseIndex + i);
        indices.push_back(baseIndex + (i + 1) % this->resolution);
    }

    // Posizionamento dei vertici della base superiore
    startIndex = static_cast<unsigned int>(vertices.size());
    vertices.push_back(Vertex{.position=glm::vec3(0, height, 0), .normal=glm::vec3(0.0f, 1.0f, 0.0f), .texCoords=glm::vec2(0, 0)});
    baseIndex = static_cast<unsigned int>(vertices.size());

    for (int i = 0; i < this->resolution; i++) {
        glm::vec3 v(r * cos(i * increment), height, r * sin(i * increment));
        vertices.push_back(Vertex{.position=v, .normal=glm::vec3(0.0f, 1.0f, 0.0f), .texCoords=glm::vec2(0, 0)});
    }
    for (int i = 0; i < this->resolution; i++) {
        indices.push_back(startIndex);
        indices.push_back(baseIndex + (i + 1) % this->resolution);
        indices.push_back(baseIndex + i);
    }

    std::vector<Texture> textures;
    textures.push_back(Texture{.id = this->tID, .type = ""});

    this->mesh = make_shared<Mesh>(vertices, indices, textures);
}

std::shared_ptr<Mesh> Branch::getResult() {
    return this->mesh;
}