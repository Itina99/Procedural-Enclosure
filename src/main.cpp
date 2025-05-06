//
// Created by Niccolo on 31/03/2025.
//
#include <iostream>
#include <cstdio>
#include <map>
#include <memory>
#include <set>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "branch_builder.h"
#include "utils.h"
#include "camera.h"
#include "interpreter.h"
#include "leaf_builder.h"
#include "lindenmayer.h"
#include "NoiseGenerator.h"
#include "shader.h"
#include "PoissonGenerator.h"
#include "tree.h"

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
    stbi_set_flip_vertically_on_load(true);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PreceduralEnclosures", nullptr, nullptr);
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
    auto skyShader = Shader("../shaders/skyBox.vert", "../shaders/skyBox.frag");


    // Create a Noise generator
    const BiomeSettings biomeSettings = noiseGen.biomePresets["Islands"];
    noiseGen.setBiome(biomeSettings);
    shader.use();
    shader.setInt("biomeId", biomeSettings.id);
    shader.setFloat("maxAmplitude", biomeSettings.amplitude);

    std::cout << "Biome ID: " << biomeSettings.id << std::endl;

    const std::vector<Texture> textures = chooseTextures(biomeSettings.id);


    // Create a Noise map
    const Mesh elevation = noiseGen.generateMesh(20, 20, textures);
    // Create a Poisson map
    std::vector<Point> treePos = PoissonGenerator::generatePositions(elevation, 20, 20, 5.0f, 20, biomeSettings.id, biomeSettings.amplitude);
    std::cout << "Generated " << treePos.size() << " points" << std::endl;

    //Create a mesh cube
    const std::vector<Vertex> vertices = {
        {{-1.0f,  1.0f, -1.0f}}, // 0: Top Left Back
        {{-1.0f, -1.0f, -1.0f}}, // 1: Bottom Left Back
        {{ 1.0f, -1.0f, -1.0f}}, // 2: Bottom Right Back
        {{ 1.0f,  1.0f, -1.0f}}, // 3: Top Right Back
        {{-1.0f,  1.0f,  1.0f}}, // 4: Top Left Front
        {{-1.0f, -1.0f,  1.0f}}, // 5: Bottom Left Front
        {{ 1.0f, -1.0f,  1.0f}}, // 6: Bottom Right Front
        {{ 1.0f,  1.0f,  1.0f}}  // 7: Top Right Front
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
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Right.bmp",   // +X
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Left.bmp",    // -X
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Top.bmp",     // +Y
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Bottom.bmp",  // -Y
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Front.bmp",   // +Z
        "../textures/SkyBox/Daylight Box_Pieces/Daylight Box_Back.bmp"     // -Z
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    std::vector<Texture> skyboxTextures = {
            {cubemapTexture, "texture_cubemap"}
    };

    const Mesh skybox(vertices, indices, skyboxTextures);
    skyShader.use();
    skyShader.setInt("skybox", 0);

    // L-System tree creation
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    Shader t_shader = Shader("../shaders/vshader.glsl", "../shaders/fshader.glsl");
    std::set<char> characters = {'P', 'F', 'L', '+', '-', '&', '^', '/', '\\', '[', ']', 'X'};
    std::map<char, std::vector<std::string>> production_rules ={
        {'P', std::vector<std::string> {"[&F[&&L]P[]F]/////[&F[&&L]P]///////[&F[&&L]P]", "[&F[&&L]P]/////////[&F[&&L]P]"}},
        {'F', std::vector<std::string> {"X/////F", "XPF", "FF", "F", "FXP"}},
        {'X', std::vector<std::string> {"F"}}
    };

    auto l = Lindenmayer(characters, production_rules);

    std::vector<Tree> forest {};

    std::shared_ptr<Branch> sBranch = std::make_shared<Branch>(50);
    std::shared_ptr<Leaf> sLeaf = std::make_shared<Leaf>();
    Interpreter turtle = Interpreter(sBranch, sLeaf, 22.5f);

    for (auto pos : treePos) {
        turtle.reset_interpreter(glm::vec3(0));
        std::vector<Mesh> meshes {};
        std::vector<glm::mat4> transforms {};

        auto result = l.generate("F", 5, true);
        turtle.read_string(result, meshes, transforms);
        forest.emplace_back(meshes, transforms);
    }

    // Check for OpenGL errors BEFORE entering the render loop
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);


        glm::mat4 view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);

        //skybox always first
        glDepthMask(GL_FALSE);   // NON scrivere nel depth buffer
        glDisable(GL_DEPTH_TEST);
        skyShader.use();
        glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyShader.setMat4("view", viewNoTranslate);
        skyShader.setMat4("projection", projection);
        skybox.render(skyShader);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        // Stuff
        shader.use();
        shader.setMat4("model", model); // identity for terrain
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("dirLight.direction", glm::vec3(-0.3f, -1.0f, -0.3f));
        shader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f)); // era 0.2
        shader.setVec3("dirLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f)); // era 0.5
        shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // ok
        shader.setVec3("viewPos", camera.position);
        shader.setFloat("time", static_cast<float>(glfwGetTime()));


        elevation.render(shader);
        t_shader.use();
        t_shader.setMat4("view", view);
        t_shader.setMat4("projection", projection);
        t_shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        t_shader.setVec3("viewPos", camera.position);
        t_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        t_shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);

        for (int i = 0; i < forest.size(); i++) {
            const float y = elevation.getHeight(treePos[i].x, treePos[i].y);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(treePos[i].x, y, treePos[i].y));
            model = glm::scale(model, glm::vec3(0.2f));
            forest[i].render(t_shader, model);
        }



         // <- Ripristina winding normale

        // events and swap buffers
        glfwSwapBuffers(window); // swaps color buffer used to render to
        glfwPollEvents(); // Checking if events are triggered, updates window state and calls callbacks
    }


    // Deallocating window
    glfwDestroyWindow(window); // Destroy the window

    glfwTerminate(); // Terminate glfw, cleaning everything up
    return 0;
}
