//
// Created by mattetina on 31/03/25.
//
#include "NoiseGenerator.h"

// Costruttore
NoiseGenerator::NoiseGenerator(): amplitude(0), sharpness(1.0f), frequency(0), warpAmp(0), warpFreq(0) {}


// Funzione per generare Perlin Noise 2D
float NoiseGenerator::generateNoise(const double x, const double y) const {
    return noise.GetNoise(x, y);
}

void NoiseGenerator::setBiome(const BiomeSettings& settings) {
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

auto NoiseGenerator::generateMesh(const int width, const int height) const -> Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

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
            const float rawNoise = noise.GetNoise(nx, nz);  // Valore in [-1.0, 1.0]
            float normalizedNoise = (rawNoise + 1.0f) * 0.5f;  // Valore in [0.0, 1.0]
            const float falloff = edgeFalloff(static_cast<float>(x), static_cast<float>(z), width, height);
            normalizedNoise *= falloff;

            float noiseValue = normalizedNoise * amplitude;

            // Applica "sharpness" per controllare la curva
            if (sharpness != 1.0f) {
                noiseValue = std::pow(noiseValue, sharpness);
            }

            // Inserisce il vertice
            vertices.push_back({ glm::vec3(x, noiseValue, z) });
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

    return {vertices, indices};
}

auto NoiseGenerator::edgeFalloff(const float x, const float z, const int width, const int height) const -> float {
    const float nx = (2.0f * x / (width - 1)) - 1.0f;   // da 0 → 1 → -1 → 1
    const float nz = (2.0f * z / (height - 1)) - 1.0f;
    const float dist = std::max(std::abs(nx), std::abs(nz)); // square mask (o usa length per sfera)

    const float falloff = 1.0f - std::pow(dist, 3.0f); // curva più morbida verso l'esterno
    return std::clamp(falloff, 0.0f, 1.0f);      // clamp per sicurezza
}




