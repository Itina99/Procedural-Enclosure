//
// Created by mattetina on 31/03/25.
//
#include "NoiseGenerator.h"

// Costruttore
NoiseGenerator::NoiseGenerator(): amplitude(0), sharpness(1.0f), frequency(0), warpAmp(0), warpFreq(0) {
}


// Funzione per generare Perlin Noise 2D
float NoiseGenerator::generateNoise(const double x, const double y) const {
    return noise.GetNoise(x, y);
}

void NoiseGenerator::setBiome(const BiomeSettings &settings) {
    noise.SetSeed(static_cast<int>(time(nullptr)));
    noise.SetFractalType(settings.fractalType);
    noise.SetFractalOctaves(settings.octaves);
    noise.SetFractalLacunarity(settings.lacunarity);
    noise.SetFractalGain(settings.gain);
    noise.SetFrequency(settings.frequency);
    this->amplitude = settings.amplitude;
    this->sharpness = settings.sharpness;
    this->frequency = settings.frequency;
    this->warpAmp = settings.warpAmp;
    this->warpFreq = settings.warpFreq;
    if (settings.warpAmp > 0.0f && settings.warpFreq > 0.0f) {
        warp.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2);
        warp.SetDomainWarpAmp(settings.warpAmp);
        warp.SetFrequency(settings.warpFreq);
    }
}

auto NoiseGenerator::generateMesh(const int width, const int height, const std::vector<Texture> &textures) const -> Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::vector<std::vector<float> > heightMap(width, std::vector<float>(height));

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            constexpr float scale = 1.3f;
            float nx = static_cast<float>(x) * scale;
            float nz = static_cast<float>(z) * scale;

            // Domain warp se richiesto
            if (warpAmp > 0.0f && warpFreq > 0.0f) {
                warp.DomainWarp(nx, nz);
            }

            // Generazione del valore di rumore
            const float rawNoise = noise.GetNoise(nx, nz); // Valore in [-1.0, 1.0]
            float normalizedNoise = (rawNoise + 1.0f) * 0.5f; // Valore in [0.0, 1.0]
            const float falloff = edgeFalloff(static_cast<float>(x), static_cast<float>(z), width, height);
            normalizedNoise *= falloff;

            float noiseValue = normalizedNoise * amplitude;

            // Applica "sharpness" per controllare la curva
            if (sharpness != 1.0f) {
                noiseValue = std::pow(noiseValue, sharpness);
            }
            heightMap[x][z] = noiseValue;
        }
    }

    // Ora costruiamo i vertici
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float y = heightMap[x][z];

            // Calcolo normale tramite differenze finite
            float left = x > 0 ? heightMap[x - 1][z] : y;
            float right = x < width - 1 ? heightMap[x + 1][z] : y;
            float down = z > 0 ? heightMap[x][z - 1] : y;
            float up = z < height - 1 ? heightMap[x][z + 1] : y;

            glm::vec3 dx = glm::vec3(1.0f, right - left, 0.0f);
            glm::vec3 dz = glm::vec3(0.0f, up - down, 1.0f);
            glm::vec3 normal = glm::normalize(glm::cross(dz, dx));

            // Coordinate texture normalizzate
            glm::vec2 texCoords = glm::vec2(
                static_cast<float>(x) / (width - 1),
                static_cast<float>(z) / (height - 1)
            );

            vertices.push_back({
                glm::vec3(x, y, z),
                normal,
                texCoords
            });
        }
    }


    // Generazione degli indici
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            const int topLeft = z * width + x;
            const int topRight = topLeft + 1;
            const int bottomLeft = topLeft + width;
            const int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triangle 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    return {vertices, indices, textures};
}

auto NoiseGenerator::edgeFalloff(const float x, const float z, const int width, const int height) const -> float {
    const float nx = (2.0f * x / (width - 1)) - 1.0f; // da 0 → 1 → -1 → 1
    const float nz = (2.0f * z / (height - 1)) - 1.0f;
    const float dist = std::max(std::abs(nx), std::abs(nz)); // square mask (o usa length per sfera)

    const float falloff = 1.0f - std::pow(dist, 3.0f); // curva più morbida verso l'esterno
    return std::clamp(falloff, 0.0f, 1.0f); // clamp per sicurezza
}
