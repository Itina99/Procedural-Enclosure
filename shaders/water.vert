#version 330 core
layout(location = 0) in vec3 position;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float waveFrequency;
uniform float waveAmplitude;
uniform float waveSpeed;

void main() {
    texCoords = position.xz * 0.1; // scale texture

    // Calcola la distanza dal centro del piano (20x20 -> centro è a (10,10))
    float dist = distance(position.xz, vec2(10.0, 10.0));

    // Calcola la deformazione verticale basata su onde concentriche
    float height = sin(dist * waveFrequency - time * waveSpeed) * waveAmplitude;

    // Applica la deformazione all'altezza (asse Y)
    vec3 displacedPosition = position + vec3(0.0, height, 0.0);

    gl_Position = projection * view * model * vec4(displacedPosition, 1.0);
}

