//
// Created by Niccolo on 02/05/2025.
//

#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include <vector>

#include "branch_builder.h"
#include "leaf_builder.h"
#include "mesh.h"


class Tree {
public:
    Tree(const std::vector<glm::mat4> &transf, const std::vector<char> &mods, std::shared_ptr<Mesh> branch, std::shared_ptr<Mesh> leaf, std::shared_ptr<Mesh> end, std::shared_ptr<Mesh> junc);
    void render(Shader &shader, const glm::mat4 &model = glm::mat4(1.0f));
private:
    std::vector<glm::mat4> transforms;
    std::vector<char> model;
    std::shared_ptr<Mesh> branch_ptr;
    std::shared_ptr<Mesh> leaf_ptr;
    std::shared_ptr<Mesh> end_ptr;
    std::shared_ptr<Mesh> junc_ptr;
};


#endif //TREE_H
