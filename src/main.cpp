//
// Created by Niccolo on 31/03/2025.
//
#include <iostream>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "utils.h"
#include "camera.h"
#include "NoiseGenerator.h"
#include "shader.h"
#include "PoissonGenerator.h"

// FPS related values
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(10.0f, 7.0f, 30.0f));

auto noiseGen = NoiseGenerator();
constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

// MousePos related values
float lastx = SCR_WIDTH / 2.0f;
float lasty = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

int main() {
    glfwInit(); //Initialization of GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ProvaOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // setting callback to resize viewport when resizing window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // This should be after window context is selected
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Allowing mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    // Setting depth test
    glEnable(GL_DEPTH_TEST);



    auto shader = Shader("../shaders/noise.vert", "../shaders/noise.frag");
    auto cubeShader = Shader("../shaders/cube.vert", "../shaders/cube.frag");


    // Create a Noise generator
    const BiomeSettings biomeSettings = noiseGen.biomePresets["Hills"];
    noiseGen.setBiome(biomeSettings);
    shader.use();
    shader.setInt("biomeId", biomeSettings.id);
    shader.setFloat("maxAmplitude", biomeSettings.amplitude);

    std::cout << "Biome ID: " << biomeSettings.id << std::endl;

    const std::vector<Texture> textures = chooseTextures(biomeSettings.id);


    // Create a Noise map
    const Mesh elevation = noiseGen.generateMesh(20, 20, textures);
    // Create a Poisson map
    std::vector<Point> treePos = PoissonGenerator::generatePositions(elevation, 20, 20, 1.0f, 20, biomeSettings.id, biomeSettings.amplitude);
    std::cout << "Generated " << treePos.size() << " points" << std::endl;

    //Create a mesh cube
    const std::vector<Vertex> vertices = {
            {{0.5f, 0.5f, 0.5f}},
            {{-0.5f, 0.5f, 0.5f}},
            {{-0.5f, -0.5f, 0.5f}},
            {{0.5f, -0.5f, 0.5f}},
            {{0.5f, -0.5f, -0.5f}},
            {{-0.5f, -0.5f, -0.5f}},
            {{-0.5f, 0.5f, -0.5f}},
            {{0.5f, 0.5f, -0.5f}}
    };
    //index buffer
    const std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            3, 2, 5,
            3, 5, 4,
            4, 5, 6,
            4, 6, 7,
            7, 6, 1,
            7, 1, 0,
            1, 6, 5,
            1, 5, 2,
            4, 7, 0,
            4, 0, 3
    };


    const Mesh cube(vertices, indices, {});


    // ✅ Check for OpenGL errors BEFORE entering the render loop
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }


    // drawing until window closes RENDER LOOP
    while (!glfwWindowShouldClose(window)) {
        // Time calculations
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear screen
        // setting clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Inputs
        processInput(window);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
                                                static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);

        // Stuff
        shader.use();
        shader.setMat4("model", model); // identity for terrain
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.setVec3("viewPos", camera.position);
        shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 8.0f);

        elevation.render(shader);

        // Draw the trees
        cubeShader.use(); // NOW switch to cube shader
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        for (const auto& pos : treePos) {
            const float y = elevation.getHeight(pos.x, pos.y);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(pos.x, y, pos.y));
            model = glm::scale(model, glm::vec3(0.2f));
            cubeShader.setMat4("model", model);
            cube.render(cubeShader);
        }

        // events and swap buffers
        glfwSwapBuffers(window); // swaps color buffer used to render to
        glfwPollEvents(); // Checking if events are triggered, updates window state and calls callbacks
    }


    // Deallocating window
    glfwDestroyWindow(window); // Destroy the window

    glfwTerminate(); // Terminate glfw, cleaning everything up
    return 0;
}
