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
    auto waterShader = Shader("../shaders/water.vert", "../shaders/water.frag");
    auto boxShader = Shader("../shaders/box.vert", "../shaders/box.frag");


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
    std::vector<Point> treePos = PoissonGenerator::generatePositions(elevation, 20, 20, 5.0f, 20, biomeSettings.id,
                                                                     biomeSettings.amplitude);
    std::cout << "Generated " << treePos.size() << " points" << std::endl;

    const Mesh skybox = setSkyBox();
    //water quad
    Mesh Water;
    if (biomeSettings.id == 4)
        Water = setWater();

    const Mesh wall = setWall();
    boxShader.use();


    // L-System tree creation
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    auto t_shader = Shader("../shaders/vshader.glsl", "../shaders/fshader.glsl");
    std::set<char> characters = {'P', 'F', 'L', '+', '-', '&', '^', '/', '\\', '[', ']', 'X'};
    std::map<char, std::vector<std::string> > production_rules = {
        {
            'P',
            std::vector<std::string>{"[&F[&&L]P[]F]/////[&F[&&L]P]///////[&F[&&L]P]", "[&F[&&L]P]/////////[&F[&&L]P]"}
        },
        {'F', std::vector<std::string>{"X/////F", "XPF", "FF", "F", "FXP"}},
        {'X', std::vector<std::string>{"F"}}
    };

    auto l = Lindenmayer(characters, production_rules);

    std::vector<Tree> forest{};

    auto sBranch = std::make_shared<Branch>(50);
    auto sLeaf = std::make_shared<Leaf>();
    auto turtle = Interpreter(sBranch, sLeaf, 22.5f);

    for (auto pos: treePos) {
        turtle.reset_interpreter(glm::vec3(0));
        std::vector<Mesh> meshes{};
        std::vector<glm::mat4> transforms{};

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
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
                                                static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
                                                100.0f);

        glm::mat4 view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);

        //skybox always first
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        skyShader.use();
        auto viewNoTranslate = glm::mat4(glm::mat3(camera.GetViewMatrix()));
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

        if (biomeSettings.id == 4) {
            waterShader.use();
            waterShader.setMat4("view", view);
            waterShader.setMat4("projection", projection);
            waterShader.setFloat("time", static_cast<float>(glfwGetTime()));
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.45f, 0.0f)); // Traslazione per posizionare sopra
            waterShader.setMat4("model", glm::scale(model, glm::vec3(1.0f))); // Traslazione per posizionare sopra
            waterShader.setFloat("waveFrequency", 3.0f); // Più alto = onde più fitte
            waterShader.setFloat("waveAmplitude", 0.05f); // Più alto = onde più alte
            waterShader.setFloat("waveSpeed", 1.5f); // Più alto = onde più veloci
            Water.render(waterShader);
        }
        //walls
        boxShader.use();
        boxShader.setMat4("view", view);
        boxShader.setMat4("projection", projection);
        boxShader.setVec3("dirLight.direction", glm::vec3(-0.3f, -1.0f, -0.3f));
        boxShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f)); // era 0.2
        boxShader.setVec3("dirLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f)); // era 0.5
        boxShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // ok
        boxShader.setFloat("material.shininess", 32.0f);
        boxShader.setVec3("viewPos", camera.position);
        for (int i = 0; i<4; i++) {
            model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
            if( i == 0) model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -0.5f));
            else if (i == 1) model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
            else if (i == 2) model = glm::translate(model, glm::vec3(-19.5f, 0.0f, -19.5f));
            else if (i == 3) model = glm::translate(model, glm::vec3(19.0f, 0.0f, -19.5f));
            boxShader.setMat4("model", model);
            wall.render(boxShader);
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
