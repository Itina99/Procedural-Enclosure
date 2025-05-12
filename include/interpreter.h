//
// Created by Niccolo on 29/04/2025.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stack>

#include "abstract_builder.h"
#include "branch_builder.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "leaf_builder.h"

struct TurtleState {
    // Per la traslazione
    glm::vec3 position;
    // Per rotazione
    glm::quat orientation;
    // Per trovare la direzione corretta dove far muovere la turtle
    glm::vec3 up;
    glm::vec3 forward;
    glm::vec3 right;

    // Per scala
    glm::mat4 scale_matrix;

    float step;
    float radius;
};

class Interpreter {
public:
    explicit Interpreter(float angle = 10.0f, glm::vec3 position = glm::vec3(0.0f), float radius = 0.1f, float length = 1.0f, float radius_decay = 0.9f, float length_decay = 0.9f);
    ~Interpreter() = default;

    void read_string(const std::string & predicate, std::vector<char>& models, std::vector<glm::mat4> & transforms);
    void reset_interpreter(glm::vec3 position = glm::vec3(0.0f));
private:
    float init_radius, init_length;
    float length_decay, radius_decay;

    TurtleState state;
    std::stack<TurtleState> state_stack;
    float angle;
};



#endif //INTERPRETER_H
