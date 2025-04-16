#version 460 core

out vec4 FragColor;

in float height;

uniform int biomeId; // 0 = Mountains, 1 = Hills, 2 = Plains, 3 = Desert, 4 = Islands

vec3 getBiomeColor(float h, int biomeId) {
    // Normalizza h (da [0, amplitude] a [0, 1])
    h = clamp(h, 0.0, 1.0); // Assumi già normalizzato, altrimenti dividi per amplitude

    if (biomeId == 0) { // Mountains
                        if (h > 0.8) return vec3(1.0, 1.0, 1.0);         // Snow
                        else if (h > 0.5) return vec3(0.5, 0.5, 0.5);    // Rock
                        else if (h > 0.4) return vec3(0.4, 0.3, 0.2);    // Grass
                        else return vec3(0.2, 0.6, 0.2);                 // Dirt
    }
    else if (biomeId == 1) { // Hills
                             if (h > 0.9) return vec3(0.5, 0.8, 0.5);         // Light green
                             else if (h > 0.4) return vec3(0.3, 0.7, 0.3);    // Lush green
                             else if (h > 0.1) return vec3(0.2, 0.6, 0.2);    // Dark green
                             else return vec3(0.1, 0.4, 0.1);                // Deep green
    }
    else if (biomeId == 2) { // Plains
                             if (h > 0.6) return vec3(0.3, 0.7, 0.3);         // Lush green
                             else if (h > 0.3) return vec3(0.5, 0.8, 0.4);    // Light grass
                             else if (h > 0.1) return vec3(0.4, 0.3, 0.2);    // Dirt
                             else return vec3(0.0, 0.3, 0.5);                 // Water
    }
    else if (biomeId == 3) { // Desert
                             if (h > 0.7) return vec3(0.8, 0.7, 0.5);         // Rocky desert
                             else return vec3(0.96, 0.87, 0.70);              // Sand
    }
    else if (biomeId == 4) { // Islands
                             if (h > 0.3) return vec3(0.2, 0.7, 0.2);         // Jungle green
                             else if (h > 0.1) return vec3(0.9, 0.85, 0.6);   // Sand
                             else return vec3(0.0, 0.4, 0.7);                 // Ocean
    }

    return vec3(1.0, 0.0, 1.0); // Magenta: fallback
}


void main() {
    vec3 color = getBiomeColor(height, biomeId);
    FragColor = vec4(color, 1.0);
}
