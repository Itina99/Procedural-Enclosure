#version 460 core

layout (location = 0) in vec3 aPos;  // Position (X, Y, Z)


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float maxAmplitude; // Maximum amplitude for height normalization

out float height;  // Pass height to fragment shader

void main() {
    height = aPos.y / maxAmplitude; // maxAmplitude passato come uniform
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
