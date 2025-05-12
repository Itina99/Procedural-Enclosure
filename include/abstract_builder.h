//
// Created by Niccolo on 29/04/2025.
//

#ifndef ABSTRACT_BUILDER_H
#define ABSTRACT_BUILDER_H

#include "mesh.h"

class Drawer {
public:
    virtual void build_branch(float height, float R, float r) {}
    virtual void build_leaf(float size) {}
    virtual void build_junciton(float radius) {}

    virtual ~Drawer() = default;
    virtual std::shared_ptr<Mesh> getResult() {return std::make_shared<Mesh>();}
};

#endif //ABSTRACT_BUILDER_H
