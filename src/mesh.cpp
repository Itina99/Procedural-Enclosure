//
// Created by Niccolo on 30/03/2025.
//

#include "mesh.h"

#include <utility>
#include <cmath>
#include <iostream>
#include <glad/glad.h>



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) : vertices(std::move(vertices)),
                                            indices(std::move(indices)),
                                            textures(std::move(textures)) {
    this->setupMesh();
}

void Mesh::render(const Shader &shader) const {
    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    glBindVertexArray(0);
}

float Mesh::getHeight(const float x, const float z) const {
    int width = 0;

    // Infer width from vertex layout
    for (size_t i = 1; i < vertices.size(); ++i) {
        if (vertices[i].position.z != vertices[0].position.z) {
            width = static_cast<int>(i);
            break;
        }
    }

    if (width == 0) return 0.0f; // Mesh is invalid or not grid-shaped

    // Clamp x and z within bounds
    if (const int height = static_cast<int>(vertices.size()) / width;
        x < 0.0f || z < 0.0f || x >= width - 1 || z >= height - 1)
        return 0.0f;

    // Get integer grid cell coordinates
    const int x0 = static_cast<int>(std::floor(x));
    const int z0 = static_cast<int>(std::floor(z));
    const int x1 = x0 + 1;
    const int z1 = z0 + 1;

    // Get fractional part for interpolation
    const float sx = x - x0;
    const float sz = z - z0;

    // Get the 4 corner heights
    const float h00 = vertices[z0 * width + x0].position.y;
    const float h10 = vertices[z0 * width + x1].position.y;
    const float h01 = vertices[z1 * width + x0].position.y;
    const float h11 = vertices[z1 * width + x1].position.y;

    // Bilinear interpolation
    const float h0 = h00 + sx * (h10 - h00);
    const float h1 = h01 + sx * (h11 - h01);
    const float h = h0 + sz * (h1 - h0);

    return h;
}
