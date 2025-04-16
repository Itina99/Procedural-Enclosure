//
// Created by mattetina on 07/04/25.
//
#ifndef POISSONGENERATOR_H
#define POISSONGENERATOR_H

#pragma once

#include <vector>
#include <optional>
#include "mesh.h"

struct Point {
    float x, y;
    explicit Point(const float x = 0, const float y = 0) : x(x), y(y) {}
};

class PoissonGenerator {
public:
    static std::vector<Point> generate(float width, float height, float minDist, int newPointsCount);
    static std::vector<Point> generatePositions(const Mesh &mesh, int meshWidth, int meshHeight, float minDist, int newPointsCount, int bioId, float bioAmplitude);



private:
    static Point generateRandomPointAround(const Point& point, float minDist);
    static Point imageToGrid(const Point& p, float cellSize);
    static bool inRectangle(const Point& p, float width, float height);
    static bool inNeighbourhood(const std::vector<std::vector<std::optional<Point>>>& grid,
                                const Point& p, float minDist, float cellSize, int gridWidth, int gridHeight);
};




#endif //POISSONGENERATOR_H
