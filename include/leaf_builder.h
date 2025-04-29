//
// Created by Niccolo on 29/04/2025.
//

#ifndef LEAF_BUILDER_H
#define LEAF_BUILDER_H

#include <glm/vec4.hpp>
#include "abstract_builder.h"

class Leaf : public Drawer{
public:
    Leaf(float size = 1.0f);
    ~Leaf() = default;
    void build_leaf() override;
    Mesh getResult() override;
private:
    Mesh mesh;
    float size;
    unsigned int tID;
};



#endif //LEAF_BUILDER_H
