//
// Created by Niccolo on 29/04/2025.
//

#ifndef LEAF_BUILDER_H
#define LEAF_BUILDER_H

#include <memory>
#include <glm/vec4.hpp>
#include "abstract_builder.h"

enum Type {
    REGULAR,
    PALM
};

class Leaf : public Drawer{
public:
    Leaf(const char* texture_path, Type type);
    ~Leaf() = default;
    void build_leaf(float size) override;
    std::shared_ptr<Mesh> getResult() override;
private:
    std::shared_ptr<Mesh> mesh;
    unsigned int tID;
    Type type;
};




#endif //LEAF_BUILDER_H
