//
// Created by mattetina on 07/04/25.
//
#include "PoissonGenerator.h"

#include <cmath>
#include <random>
#include <vector>
#include <algorithm>

namespace {
    std::mt19937 rng(std::random_device{}());

    struct RandomQueue {
        void push(const Point& p) { data.push_back(p); }

        Point pop() {
            std::uniform_int_distribution<int> dist(0, static_cast<int>(data.size()) - 1);
            const int index = dist(rng);
            const Point chosen = data[index];
            data[index] = data.back();
            data.pop_back();
            return chosen;
        }

        [[nodiscard]] bool empty() const { return data.empty(); }

        std::vector<Point> data;
    };
}

std::vector<Point> PoissonGenerator::generate(const float width, const float height,const  float minDist, const int newPointsCount) {
    const float cellSize = minDist / std::sqrt(2.0f);
    const int gridWidth = static_cast<int>(std::ceil(width / cellSize));
    const int gridHeight = static_cast<int>(std::ceil(height / cellSize));

    std::vector<std::vector<std::optional<Point>>> grid(gridHeight, std::vector<std::optional<Point>>(gridWidth));
    std::vector<Point> samplePoints;
    RandomQueue processList;

    std::uniform_real_distribution<float> distX(0.0f, width);
    std::uniform_real_distribution<float> distY(0.0f, height);
    Point firstPoint(distX(rng), distY(rng));

    processList.push(firstPoint);
    samplePoints.push_back(firstPoint);
    const Point gridCoord = imageToGrid(firstPoint, cellSize);
    grid[static_cast<int>(gridCoord.y)][static_cast<int>(gridCoord.x)] = firstPoint;

    while (!processList.empty()) {
        Point point = processList.pop();
        for (int i = 0; i < newPointsCount; ++i) {
            if (Point newPoint = generateRandomPointAround(point, minDist); inRectangle(newPoint, width, height) &&
                                                                            !inNeighbourhood(grid, newPoint, minDist, cellSize, gridWidth, gridHeight)) {
                samplePoints.push_back(newPoint);
                processList.push(newPoint);
                const Point newGrid = imageToGrid(newPoint, cellSize);
                grid[static_cast<int>(newGrid.y)][static_cast<int>(newGrid.x)] = newPoint;
            }
        }
    }

    return samplePoints;
}



Point PoissonGenerator::generateRandomPointAround(const Point& point, const float minDist) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    const float r1 = dist(rng);
    const float r2 = dist(rng);

    const float radius = minDist * (r1 + 1.0f);
    const float angle = 2.0f * static_cast<float>(M_PI) * r2;

    const float newX = point.x + radius * std::cos(angle);
    const float newY = point.y + radius * std::sin(angle);

    return Point{newX, newY};
}

Point PoissonGenerator::imageToGrid(const Point& p, float cellSize) {
    return Point{static_cast<float>(static_cast<int>(std::floor(p.x / cellSize))),
                 static_cast<float>(static_cast<int>(std::floor(p.y / cellSize)))};
}

bool PoissonGenerator::inRectangle(const Point& p,const  float width,const  float height) {
    return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
}

bool PoissonGenerator::inNeighbourhood(
    const std::vector<std::vector<std::optional<Point>>>& grid,
    const Point& p,const  float minDist, const float cellSize,
    const int gridWidth, const int gridHeight) {

    const Point gridPos = imageToGrid(p, cellSize);

    const int startX = std::max(0, static_cast<int>(gridPos.x) - 2);
    const int endX = std::min(gridWidth - 1, static_cast<int>(gridPos.x) + 2);
    const int startY = std::max(0, static_cast<int>(gridPos.y) - 2);
    const int endY = std::min(gridHeight - 1, static_cast<int>(gridPos.y) + 2);

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (grid[y][x]) {
                const float dx = grid[y][x]->x - p.x;
                if (const float dy = grid[y][x]->y - p.y; (dx * dx + dy * dy) < (minDist * minDist)) {
                    return true;
                }
            }
        }
    }

    return false;
}

std::vector<Point> PoissonGenerator::generatePositions(const Mesh& mesh, const int meshWidth, const int meshHeight, const float minDist, const int newPointsCount, const int bioId, const float bioAmplitude) {
    std::vector<Point> points = generate(static_cast<float>(meshWidth)-1, static_cast<float>(meshHeight)-1, minDist, newPointsCount);

    // Filter points based on terrain height using Mesh::getHeight
    std::erase_if(points, [&](const Point& p) {
        const float height = mesh.getHeight(p.x, p.y) / bioAmplitude;


        switch (bioId) {
            case 0: // Mountains
                return height < 0.1 || height > 0.5f ; // Solo in zone alte
            case 1: // Hills
                return height < 0.1f || height > 0.9f;
            case 2: // Plains
                return height < 0.1f ;
            case 3: // Desert
                return height < 0.1f || height > 0.5f;
            case 4: // Islands
                return height < 0.3f || height > 0.8f;
            default: // Fallback: filtra tutto (non genera niente)
                return true;
        }
    });

    return points;
}



