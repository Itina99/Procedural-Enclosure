#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

out vec2 tCoords;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    tCoords = texCoords;
    fragPos = vec3(model * vec4(vertexPosition, 1.0));
    normal = mat3(transpose(inverse(model))) * normals;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}