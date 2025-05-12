//
// Created by Niccolo on 09/05/2025.
//

#include "junction_builder.h"

#include <glm/ext/scalar_constants.hpp>

#include "utils.h"

Junction::Junction(const char* texture_path, unsigned int resolution) : resolution(resolution) {
    this->tID = loadTexture(texture_path);
}

void Junction::build_junciton(float radius) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    double phi_increment = glm::radians(90.0f / ((float)this->resolution/2.0f));
    double theta_increment = glm::radians(360.0f / (float)this->resolution);

    for (int i = 0; i <= resolution; ++i) {
        float phi = phi_increment * i;
        for (int j = 0; j <= resolution; ++j) {
            float theta = theta_increment * j;

            glm::vec3 position;
            position.x = radius * sin(phi) * cos(theta);
            position.y = radius * cos(phi);
            position.z = radius * sin(phi) * sin(theta);

            glm::vec3 normal = glm::normalize(position);
            glm::vec2 texCoord;
            texCoord.x = static_cast<float>(j) / static_cast<float>(resolution);
            texCoord.y = static_cast<float>(i) / static_cast<float>(resolution);

            vertices.push_back({position, normal, texCoord});
        }
    }

    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            unsigned int first = (i * (resolution + 1)) + j;
            unsigned int second = first + resolution + 1;

            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second + 1);
        }
    }

    std::vector<Texture> textures;
    textures.push_back(Texture{.id = this->tID, .type = ""});

    this->mesh = make_shared<Mesh>(vertices, indices, textures);
}


std::shared_ptr<Mesh> Junction::getResult() {
    return this->mesh;
}
