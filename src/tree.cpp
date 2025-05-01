//
// Created by Niccolo on 02/05/2025.
//

#include "tree.h"

#include <utility>

Tree::Tree(const std::vector<Mesh> &meshes, const std::vector<glm::mat4> &transf) : meshes(std::move(meshes)), transforms(std::move(transf)) {
}

void Tree::render(Shader &shader, const glm::mat4 &model) {
    for (int i = 0; i < meshes.size(); i++) {
        shader.setMat4("model", model * transforms[i]);
        meshes[i].render(shader);
    }
}