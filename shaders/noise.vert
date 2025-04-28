#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float maxAmplitude;

out float height;
out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;

void main() {
    height = aPos.y / maxAmplitude;
    texCoords = aTexCoords;
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(fragPos, 1.0);
}

