//
// Created by Niccolo on 09/05/2025.
//

#ifndef JUNCTION_BUILDER_H
#define JUNCTION_BUILDER_H
#include <memory>

#include "abstract_builder.h"


class Junction : public Drawer{
public:
    Junction(const char* texture_path, unsigned int resolution = 8);
    ~Junction() override = default;

    void build_junciton(float radius) override;
    std::shared_ptr<Mesh> getResult() override;
private:
    std::shared_ptr<Mesh> mesh;
    unsigned int resolution;
    unsigned int tID;
};



#endif //JUNCTION_BUILDER_H
