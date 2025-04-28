#version 460

layout (location = 0) in vec3 aPos;  // Position (X, Y, Z)
layout (location = 1) in vec3 aNormal; // Normal (X, Y, Z)
layout (location = 2) in vec2 aTexCoord; // Texture coordinates (U, V)


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}