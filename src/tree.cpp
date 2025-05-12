//
// Created by Niccolo on 02/05/2025.
//

#include "tree.h"

#include <utility>

Tree::Tree(const std::vector<glm::mat4> &transf, const std::vector<char> &mods, std::shared_ptr<Mesh> branch,
    std::shared_ptr<Mesh> leaf, std::shared_ptr<Mesh> end, std::shared_ptr<Mesh> junc) : transforms(transf), model(mods), branch_ptr(branch), leaf_ptr(leaf), end_ptr(end), junc_ptr(junc) {
}

void Tree::render(Shader &shader, const glm::mat4 &m_matrix) {
    for (int i = 0; i < model.size(); i++) {
        shader.setMat4("model", m_matrix * transforms[i]);
        switch (auto c = model[i]) {
            case 'F': {
                branch_ptr->render(shader);
                break;
            }
            case 'L': {
                leaf_ptr->render(shader);
                break;
            }
            case 'P': {
                end_ptr->render(shader);
                break;
            }
            case 'J': {
                junc_ptr->render(shader);
            }
            default: {
                break;
            }
        }
    }
}