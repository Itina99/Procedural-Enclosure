//
// Created by Niccolo on 29/04/2025.
//

#ifndef ABSTRACT_BUILDER_H
#define ABSTRACT_BUILDER_H

#include "mesh.h"

class Drawer {
public:
    virtual void build_branch(float height, float R, float r) {}
    virtual void build_leaf() {}

    virtual ~Drawer() = default;
    virtual Mesh getResult() {return Mesh();}
};

#endif //ABSTRACT_BUILDER_H
