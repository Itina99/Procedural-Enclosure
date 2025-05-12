//
// Created by Niccolo on 29/04/2025.
//

#ifndef BRANCH_BUILDER_H
#define BRANCH_BUILDER_H

#include <memory>

#include "abstract_builder.h"

class Branch : public Drawer {
public:
    Branch(const char* texture_path, unsigned int resolution = 8);
    ~Branch() override = default;
    void build_branch(float height, float R, float r) override;
    std::shared_ptr<Mesh> getResult() override;

private:
    std::shared_ptr<Mesh> mesh;
    unsigned int resolution;
    unsigned int tID;
};



#endif //BRANCH_BUILDER_H
