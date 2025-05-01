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
    Tree(const std::vector<Mesh> &meshes, const std::vector<glm::mat4> &transf);
    void render(Shader &shader, const glm::mat4 &model = glm::mat4(1.0f));
private:
    std::vector<glm::mat4> transforms;
    std::vector<Mesh> meshes;
};


#endif //TREE_H
