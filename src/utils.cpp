//
// Created by Niccolo on 28/03/2025.
//

#include "utils.h"

#include <iostream>

#include "camera.h"
#include "mesh.h"


extern Camera camera;
extern bool firstMouse;
extern float lastx, lasty;
extern float deltaTime;


void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.movementSpeed = 5.0f;
    } else {
        camera.movementSpeed = 2.5f;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastx = xpos;
        lasty = ypos;
        firstMouse = false;
    }
    // Mouse position calculation
    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos; // reversed because coordinates are flipped
    lastx = xpos;
    lasty = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Texture> chooseTextures(const int biomeId) {
    switch (biomeId) {
        case 0: // Mountains
            return {
                {loadTexture("../textures/Snow/textures/snow_02_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Rock/rock_face_03_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Rock/aerial_rocks_02_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Rock/rocky_terrain_02_diff_1k.png"), "texture_diffuse"}
            };
        case 1: // Hills
            return {
                {loadTexture("../textures/grass/leafy_grass_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/grass/brown_mud_leaves_01_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Rock/rocky_terrain_02_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/grass/aerial_grass_rock_diff_1k.png"), "texture_diffuse"}
            };
        case 3: // Desert
            return {
                {loadTexture("../textures/Sand/rock_boulder_cracked_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Sand/sandy_gravel_02_diff_1k.png"), "texture_diffuse"},
            };
        case 4: // Islands
            return {
                {loadTexture("../textures/Rock/rocky_terrain_02_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Sand/aerial_beach_01_diff_1k.png"), "texture_diffuse"},
                {loadTexture("../textures/Waves/0012.png"), "texture_normal"},
                {loadTexture("../textures/Waves/0071.png"), "texture_normal"},


            };
        default:
            return {};
    }
}

unsigned int loadCubemap(const std::vector<std::string> &faces) {
    stbi_set_flip_vertically_on_load(false); // ← Importante per cubemap

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);
    return textureID;
}

Mesh setSkyBox() {
    //Create a mesh cube
    const std::vector<Vertex> vertices = {
        {{-1.0f, 1.0f, -1.0f}}, // 0: Top Left Back
        {{-1.0f, -1.0f, -1.0f}}, // 1: Bottom Left Back
        {{1.0f, -1.0f, -1.0f}}, // 2: Bottom Right Back
        {{1.0f, 1.0f, -1.0f}}, // 3: Top Right Back
        {{-1.0f, 1.0f, 1.0f}}, // 4: Top Left Front
        {{-1.0f, -1.0f, 1.0f}}, // 5: Bottom Left Front
        {{1.0f, -1.0f, 1.0f}}, // 6: Bottom Right Front
        {{1.0f, 1.0f, 1.0f}} // 7: Top Right Front
    };

    const std::vector<unsigned int> indices = {
        // Back face
        0, 1, 2,
        0, 2, 3,

        // Front face
        7, 6, 5,
        7, 5, 4,

        // Left face
        4, 5, 1,
        4, 1, 0,

        // Right face
        3, 2, 6,
        3, 6, 7,

        // Bottom face
        1, 5, 6,
        1, 6, 2,

        // Top face
        4, 0, 3,
        4, 3, 7
    };

    std::vector<std::string> faces
    {
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Right.bmp", // +X
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Left.bmp", // -X
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Top.bmp", // +Y
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Bottom.bmp", // -Y
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Front.bmp", // +Z
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Back.bmp" // -Z
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    std::vector<Texture> skyboxTextures = {
        {cubemapTexture, "texture_cubemap"}
    };

    return {vertices, indices, skyboxTextures};
}

Mesh setWater() {
    const std::vector<Vertex> waterVertices{
        {{0.0f, 0.0f, 0.0f}},
        {{19.0f, 0.0f, 0.0f}},
        {{19.0f, 0.0f, 19.0f}},
        {{0.0f, 0.0f, 19.0f}}
    };

    const std::vector<unsigned int> waterIndices = {
        0, 2, 1,
        0, 3, 2
    };
    std::vector<Texture> waterTextures = {
        {loadTexture("../textures/Waves/0012.png"), "texture_normal"}
    };

    return {waterVertices, waterIndices, waterTextures};
}

Mesh setWall() {
    const std::vector<Vertex> vertices = {
        // Bottom face
        {{0.0f, 0.0f, 0.0f}, {0, -1, 0}, {0.0f, 0.0f}}, // 0
        {{0.5f, 0.0f, 0.0f}, {0, -1, 0}, {1.0f, 0.0f}}, // 1
        {{0.5f, 0.0f, 20.0f}, {0, -1, 0}, {1.0f, 20.0f}}, // 2
        {{0.0f, 0.0f, 20.0f}, {0, -1, 0}, {0.0f, 20.0f}}, // 3

        // Top face
        {{0.0f, 1.0f, 0.0f}, {0, 1, 0}, {0.0f, 0.0f}}, // 4
        {{0.5f, 1.0f, 0.0f}, {0, 1, 0}, {1.0f, 0.0f}}, // 5
        {{0.5f, 1.0f, 20.0f}, {0, 1, 0}, {1.0f, 20.0f}}, // 6
        {{0.0f, 1.0f, 20.0f}, {0, 1, 0}, {0.0f, 20.0f}}, // 7

        // Front face
        {{0.0f, 0.0f, 20.0f}, {0, 0, 1}, {0.0f, 0.0f}}, // 8
        {{0.5f, 0.0f, 20.0f}, {0, 0, 1}, {1.0f, 0.0f}}, // 9
        {{0.5f, 1.0f, 20.0f}, {0, 0, 1}, {1.0f, 1.0f}}, // 10
        {{0.0f, 1.0f, 20.0f}, {0, 0, 1}, {0.0f, 1.0f}}, // 11

        // Back face
        {{0.0f, 0.0f, 0.0f}, {0, 0, -1}, {0.0f, 0.0f}}, // 12
        {{0.5f, 0.0f, 0.0f}, {0, 0, -1}, {1.0f, 0.0f}}, // 13
        {{0.5f, 1.0f, 0.0f}, {0, 0, -1}, {1.0f, 1.0f}}, // 14
        {{0.0f, 1.0f, 0.0f}, {0, 0, -1}, {0.0f, 1.0f}}, // 15

        // Left face
        {{0.0f, 0.0f, 0.0f}, {-1, 0, 0}, {0.0f, 0.0f}}, // 16
        {{0.0f, 0.0f, 20.0f}, {-1, 0, 0}, {20.0f, 0.0f}}, // 17
        {{0.0f, 1.0f, 20.0f}, {-1, 0, 0}, {20.0f, 1.0f}}, // 18
        {{0.0f, 1.0f, 0.0f}, {-1, 0, 0}, {0.0f, 1.0f}}, // 19

        // Right face
        {{0.5f, 0.0f, 0.0f}, {1, 0, 0}, {0.0f, 0.0f}}, // 20
        {{0.5f, 0.0f, 20.0f}, {1, 0, 0}, {20.0f, 0.0f}}, // 21
        {{0.5f, 1.0f, 20.0f}, {1, 0, 0}, {20.0f, 1.0f}}, // 22
        {{0.5f, 1.0f, 0.0f}, {1, 0, 0}, {0.0f, 1.0f}}, // 23
    };

    const std::vector<unsigned int> indices = {
        // bottom
        0, 1, 2, 0, 2, 3,
        // top
        4, 6, 5, 4, 7, 6,
        // front
        8, 9, 10, 8, 10, 11,
        // back
        12, 14, 13, 12, 15, 14,
        // left
        16, 17, 18, 16, 18, 19,
        // right
        20, 22, 21, 20, 23, 22
    };


    std::vector<Texture> textures = {
        {loadTexture("../textures/Walls/wooden_garage_door_diff_1k.png"), "texture_diffuse"},
        {loadTexture("../textures/Walls/wooden_garage_door_arm_1k.png"), "texture_specular"}

    };
    // {loadTexture("../textures/Walls/wooden_garage_door_spec_1k.png"), "texture_specular"},
    for (auto vertex: textures) {
    }
    return {vertices, indices, textures};
}
