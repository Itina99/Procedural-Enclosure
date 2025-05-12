//
// Created by Niccolo on 29/04/2025.
//

#include "leaf_builder.h"

#include "utils.h"

Leaf::Leaf(const char* texture_path, Type type): type(type){
    this->tID = loadTexture(texture_path);
}

void Leaf::build_leaf(float size) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    if (this->type == 0) {
        glm::vec3 v1 = glm::vec3(-size/2, 0.0f, 0.0f);
        glm::vec3 v2 = glm::vec3(size/2, 0.0f, 0.0f);
        glm::vec3 v3 = glm::vec3(-size/2, size, 0.0f);
        glm::vec3 v4 = glm::vec3(size/2, size, 0.0f);

        vertices.push_back(Vertex{.position=v1, .normal=glm::vec3(0.0f, 0.0f, -1.0f), .texCoords = glm::vec2(0.0f, 0.06f)});
        vertices.push_back(Vertex{.position=v2, .normal=glm::vec3(0.0f, 0.0f, -1.0f), .texCoords = glm::vec2(1.0f, 0.06f)});
        vertices.push_back(Vertex{.position=v3, .normal=glm::vec3(0.0f, 0.0f, -1.0f), .texCoords = glm::vec2(0.0f, 1.0f)});
        vertices.push_back(Vertex{.position=v4, .normal=glm::vec3(0.0f, 0.0f, -1.0f), .texCoords = glm::vec2(1.0f, 1.0f)});

        vertices.push_back(Vertex{.position=v1, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(0.0f, 0.06f)});
        vertices.push_back(Vertex{.position=v2, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(1.0f, 0.06f)});
        vertices.push_back(Vertex{.position=v3, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(0.0f, 1.0f)});
        vertices.push_back(Vertex{.position=v4, .normal=glm::vec3(0.0f, 0.0f, 1.0f), .texCoords = glm::vec2(1.0f, 1.0f)});

        indices = {0, 2, 1, 2, 3, 1, 5, 7, 4, 7, 6, 4};

        std::vector<Texture> textures;
        textures.push_back(Texture{.id = this->tID, .type = ""});

        this->mesh = std::make_shared<Mesh>(vertices, indices, textures);
    }
    else {
        float last_start_z = 0.0f;
        // Lato destro della foglia di palma
        for (int i = 0; i < 4; i++) {
            glm::vec3 v1 = glm::vec3(0.0f, i * size, last_start_z);
            glm::vec3 v2 = glm::vec3(0.0f, (i+1) * size, last_start_z + (i * -0.1f));
            glm::vec3 v3 = glm::vec3(size * glm::cos(glm::radians(10.0f)), i * size, (-size * glm::sin(glm::radians(10.0f)) + last_start_z));
            glm::vec3 v4 = glm::vec3(size * glm::cos(glm::radians(10.0f)), (i+1) * size, (-size * glm::sin(glm::radians(10.0f)) + (last_start_z + (i * -0.1))));
            last_start_z += (i * -0.1f);

            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            unsigned int start = static_cast<unsigned int>(vertices.size());

            vertices.push_back(Vertex{.position=v1, .normal=normal, .texCoords = glm::vec2(0.5f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v2, .normal=normal, .texCoords = glm::vec2(0.5f, (i+1) * 0.25f)});
            vertices.push_back(Vertex{.position=v3, .normal=normal, .texCoords = glm::vec2(1.0f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v4, .normal=normal, .texCoords = glm::vec2(1.0f, (i+1) * 0.25f)});

            vertices.push_back(Vertex{.position=v1, .normal=-normal, .texCoords = glm::vec2(0.5f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v2, .normal=-normal, .texCoords = glm::vec2(0.5f, (i+1) * 0.25f)});
            vertices.push_back(Vertex{.position=v3, .normal=-normal, .texCoords = glm::vec2(1.0f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v4, .normal=-normal, .texCoords = glm::vec2(1.0f, (i+1) * 0.25f)});

            indices.push_back(start);
            indices.push_back(start + 1);
            indices.push_back(start + 2);
            indices.push_back(start + 1);
            indices.push_back(start + 3);
            indices.push_back(start + 2);
            indices.push_back(start + 4);
            indices.push_back(start + 7);
            indices.push_back(start + 5);
            indices.push_back(start + 4);
            indices.push_back(start + 6);
            indices.push_back(start + 7);
        }

        last_start_z = 0.0f;
        // Lato sinistro della foglia di palma
        for (int i = 0; i < 4; i++) {
            glm::vec3 v1 = glm::vec3(0.0f, i * size, last_start_z);
            glm::vec3 v2 = glm::vec3(0.0f, (i+1) * size, last_start_z + (i * -0.1f));
            glm::vec3 v3 = glm::vec3(-size * glm::cos(glm::radians(10.0f)), i * size, (-size * glm::sin(glm::radians(10.0f)) + last_start_z));
            glm::vec3 v4 = glm::vec3(-size * glm::cos(glm::radians(10.0f)), (i+1) * size, (-size * glm::sin(glm::radians(10.0f)) + (last_start_z + (i * -0.1))));
            last_start_z += (i * -0.1f);

            glm::vec3 normal = glm::normalize(glm::cross(v3 - v1, v2 - v1));
            unsigned int start = static_cast<unsigned int>(vertices.size());

            vertices.push_back(Vertex{.position=v1, .normal=normal, .texCoords = glm::vec2(0.5f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v2, .normal=normal, .texCoords = glm::vec2(0.5f, (i+1) * 0.25f)});
            vertices.push_back(Vertex{.position=v3, .normal=normal, .texCoords = glm::vec2(0.0f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v4, .normal=normal, .texCoords = glm::vec2(0.0f, (i+1) * 0.25f)});

            vertices.push_back(Vertex{.position=v1, .normal=-normal, .texCoords = glm::vec2(0.5f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v2, .normal=-normal, .texCoords = glm::vec2(0.5f, (i+1) * 0.25f)});
            vertices.push_back(Vertex{.position=v3, .normal=-normal, .texCoords = glm::vec2(0.0f, i * 0.25f)});
            vertices.push_back(Vertex{.position=v4, .normal=-normal, .texCoords = glm::vec2(0.0f, (i+1) * 0.25f)});

            indices.push_back(start);
            indices.push_back(start + 2);
            indices.push_back(start + 3);
            indices.push_back(start);
            indices.push_back(start + 3);
            indices.push_back(start + 1);
            indices.push_back(start + 4);
            indices.push_back(start + 7);
            indices.push_back(start + 6);
            indices.push_back(start + 4);
            indices.push_back(start + 5);
            indices.push_back(start + 7);
        }

        std::vector<Texture> textures;
        textures.push_back(Texture{.id = this->tID, .type = ""});

        this->mesh = std::make_shared<Mesh>(vertices, indices, textures);
    }

}

std::shared_ptr<Mesh> Leaf::getResult() {
    return this->mesh;
}
