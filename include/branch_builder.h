//
// Created by Niccolo on 29/04/2025.
//

#ifndef BRANCH_BUILDER_H
#define BRANCH_BUILDER_H

#include "abstract_builder.h"

class Branch : public Drawer {
public:
    Branch(unsigned int resolution = 8);
    ~Branch() = default;
    void build_branch(float height, float R, float r) override;
    Mesh getResult() override;

private:
    Mesh mesh;
    unsigned int resolution;
    unsigned int tID;
};



#endif //BRANCH_BUILDER_H
