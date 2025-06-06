//
// Created by mattetina on 31/03/25.
//

#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "mesh.h"
#include "../lib/FastNoiseLite.h"

struct BiomeSettings {
    float frequency;
    float amplitude;
    FastNoiseLite::FractalType fractalType;
    int octaves;
    float lacunarity;
    float gain;
    int id;
    float sharpness; // (opzionale, default 1.0 = nessuna modifica)
    float warpAmp; // (opzionale, default 0.0 = nessuna modifica)
    float warpFreq; // (opzionale, default 0.0 = nessuna modifica)
};

enum class Biomes {
    MOUNTAINS,
    HILLS,
    DESERT,
    ISLANDS
};


class NoiseGenerator {
    FastNoiseLite noise;
    FastNoiseLite warp;

    float amplitude;
    float sharpness;
    float frequency;
    float warpAmp;
    float warpFreq;

    float edgeFalloff(float x, float z, int width, int height) const;



public:
    NoiseGenerator();

    std::unordered_map<Biomes, BiomeSettings> biomePresets = {
    {Biomes::MOUNTAINS, {0.015f, 5.0f, FastNoiseLite::FractalType_Ridged, 6, 2.0f, 0.5f, 0, 1.31f, 15.0f, 0.05f}},
    {Biomes::HILLS, {0.06f, 3.0f, FastNoiseLite::FractalType_FBm, 3, 2.0f, 0.4f, 1, 0.8f, 5.0f, 0.035f}},
    {Biomes::DESERT, {0.03f, 2.0f, FastNoiseLite::FractalType_FBm, 3, 1.8f, 0.4f, 3, 1.2f, 3.0f, 0.02f}},
    {Biomes::ISLANDS, {0.05f, 1.8f, FastNoiseLite::FractalType_FBm, 6, 2.2f, 0.38f, 4, 1.6f, 25.0f, 0.05f}},
};


    void setBiome(const BiomeSettings &settings);

    float generateNoise(double x, double y) const;

    Mesh generateMesh(int width, int height, const std::vector<Texture> &textures) const;
};

#endif // NOISE_GENERATOR_H
