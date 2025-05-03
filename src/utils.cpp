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



void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
    }
    else {
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

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
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
    }
    else
    {
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

