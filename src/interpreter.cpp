//
// Created by Niccolo on 29/04/2025.
//

#include "interpreter.h"

#include <utility>

Interpreter::Interpreter(float angle, glm::vec3 position, float radius, float length, float radius_decay, float length_decay) : init_radius(radius), init_length(length), radius_decay(radius_decay), length_decay(length_decay) {

    TurtleState s = TurtleState();
    s.position = position;
    s.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    s.forward = glm::vec3(0.0f, 1.0f, 0.0f);
    s.up = glm::vec3(0.0f, 0.0f,1.0f);
    s.right = glm::vec3(1.0f, 0.0f, 0.0f);
    s.step = this->init_length;
    s.scale_matrix = glm::mat4(1.0f);
    this->angle = glm::radians(angle);
    this->state = s;
}

void Interpreter::read_string(const std::string &predicate, std::vector<char>& models, std::vector<glm::mat4>& transforms) {
    TurtleState current = this->state;
    glm::quat rot;
    glm::mat4 translation, rotation = glm::mat4(1.0f);
    for (char c : predicate) {
        switch (c) {
            case 'P': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation * current.scale_matrix);
                models.push_back('P');
                break;
            }
            case 'J': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation * glm::scale(current.scale_matrix, glm::vec3(1.0f, current.scale_matrix[0][0], 1.0f)));
                models.push_back('J');
                break;
            }
            case 'F': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation * current.scale_matrix);
                glm::vec3 movement_direction = glm::normalize(glm::vec3(current.forward.x, current.forward.y, current.forward.z));
                this->state.position = current.position + current.step * movement_direction;
                current = this->state;
                models.push_back('F');
                break;
            }
            case 'L': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation * scale(glm::mat4(1.0), glm::vec3(current.scale_matrix[1][1])));
                models.push_back('L');
                break;
            }
            case '+': {
                // Esegui la rotazione e salvala
                rot = glm::angleAxis(this->angle, current.up);
                this->state.orientation = rot * current.orientation;
                // Ruota il sistema di riferimento locale
                glm::quat up = glm::quat(0.0f, current.up);
                glm::quat right = glm::quat(0.0f, current.right);
                glm::quat forward = glm::quat(0.0f, current.forward);
                glm::quat rotated_fwd = rot * forward * glm::conjugate(rot);
                glm::quat rotated_up = rot * up * glm::conjugate(rot);
                glm::quat rotated_right = rot * right * glm::conjugate(rot);
                this->state.up = glm::normalize(glm::vec3(rotated_up.x, rotated_up.y, rotated_up.z));
                this->state.forward = glm::normalize(glm::vec3(rotated_fwd.x, rotated_fwd.y, rotated_fwd.z));
                this->state.right = glm::normalize(glm::vec3(rotated_right.x, rotated_right.y, rotated_right.z));
                current = this->state;
                break;
            }
            case '-': {
                rot = glm::angleAxis(-this->angle, current.up);
                this->state.orientation = rot * current.orientation;
                // Ruota il sistema di riferimento locale
                glm::quat up = glm::quat(0.0f, current.up);
                glm::quat right = glm::quat(0.0f, current.right);
                glm::quat forward = glm::quat(0.0f, current.forward);
                glm::quat rotated_fwd = rot * forward * glm::conjugate(rot);
                glm::quat rotated_up = rot * up * glm::conjugate(rot);
                glm::quat rotated_right = rot * right * glm::conjugate(rot);
                this->state.up = glm::normalize(glm::vec3(rotated_up.x, rotated_up.y, rotated_up.z));
                this->state.forward = glm::normalize(glm::vec3(rotated_fwd.x, rotated_fwd.y, rotated_fwd.z));
                this->state.right = glm::normalize(glm::vec3(rotated_right.x, rotated_right.y, rotated_right.z));
                current = this->state;
                break;
            }
            case '&': {
                rot = glm::angleAxis(this->angle, current.right);
                this->state.orientation = rot * current.orientation;
                glm::quat up = glm::quat(0.0f, current.up);
                glm::quat right = glm::quat(0.0f, current.right);
                glm::quat forward = glm::quat(0.0f, current.forward);
                glm::quat rotated_fwd = rot * forward * glm::conjugate(rot);
                glm::quat rotated_up = rot * up * glm::conjugate(rot);
                glm::quat rotated_right = rot * right * glm::conjugate(rot);
                this->state.up = glm::normalize(glm::vec3(rotated_up.x, rotated_up.y, rotated_up.z));
                this->state.forward = glm::normalize(glm::vec3(rotated_fwd.x, rotated_fwd.y, rotated_fwd.z));
                this->state.right = glm::normalize(glm::vec3(rotated_right.x, rotated_right.y, rotated_right.z));
                current = this->state;
                break;
            }
            case '^': {
                rot = glm::angleAxis(-this->angle, current.right);
                this->state.orientation = rot * current.orientation;
                glm::quat up = glm::quat(0.0f, current.up);
                glm::quat right = glm::quat(0.0f, current.right);
                glm::quat forward = glm::quat(0.0f, current.forward);
                glm::quat rotated_fwd = rot * forward * glm::conjugate(rot);
                glm::quat rotated_up = rot * up * glm::conjugate(rot);
                glm::quat rotated_right = rot * right * glm::conjugate(rot);
                this->state.up = glm::normalize(glm::vec3(rotated_up.x, rotated_up.y, rotated_up.z));
                this->state.forward = glm::normalize(glm::vec3(rotated_fwd.x, rotated_fwd.y, rotated_fwd.z));
                this->state.right = glm::normalize(glm::vec3(rotated_right.x, rotated_right.y, rotated_right.z));
                current = this->state;
                break;
            }
            case '/': {
                rot = glm::angleAxis(this->angle, current.forward);
                this->state.orientation = rot * current.orientation;
                glm::quat up = glm::quat(0.0f, current.up);
                glm::quat right = glm::quat(0.0f, current.right);
                glm::quat forward = glm::quat(0.0f, current.forward);
                glm::quat rotated_fwd = rot * forward * glm::conjugate(rot);
                glm::quat rotated_up = rot * up * glm::conjugate(rot);
                glm::quat rotated_right = rot * right * glm::conjugate(rot);
                this->state.up = glm::normalize(glm::vec3(rotated_up.x, rotated_up.y, rotated_up.z));
                this->state.forward = glm::normalize(glm::vec3(rotated_fwd.x, rotated_fwd.y, rotated_fwd.z));
                this->state.right = glm::normalize(glm::vec3(rotated_right.x, rotated_right.y, rotated_right.z));
                current = this->state;
                break;
            }
            case '(': {
                rot = glm::angleAxis(-this->angle, current.forward);
                this->state.orientation = rot * current.orientation;
                glm::quat up = glm::quat(0.0f, current.up);
                glm::quat right = glm::quat(0.0f, current.right);
                glm::quat forward = glm::quat(0.0f, current.forward);
                glm::quat rotated_fwd = rot * forward * glm::conjugate(rot);
                glm::quat rotated_up = rot * up * glm::conjugate(rot);
                glm::quat rotated_right = rot * right * glm::conjugate(rot);
                this->state.up = glm::normalize(glm::vec3(rotated_up.x, rotated_up.y, rotated_up.z));
                this->state.forward = glm::normalize(glm::vec3(rotated_fwd.x, rotated_fwd.y, rotated_fwd.z));
                this->state.right = glm::normalize(glm::vec3(rotated_right.x, rotated_right.y, rotated_right.z));
                current = this->state;
                break;
            }
            case '!': {
                this->state.scale_matrix = scale(this->state.scale_matrix, glm::vec3(radius_decay, 1.0f, radius_decay ));
                this->state.radius *= radius_decay;
                current = this->state;
                break;
            }
            case '%': {
                if (this->state.step * length_decay > init_length/5) {
                    this->state.scale_matrix = scale(this->state.scale_matrix, glm::vec3(1.0f, length_decay, 1.0f ));
                    this->state.step *= length_decay;
                }
                current = this->state;
                break;
            }
            case '[': {
                this->state_stack.push(current);
                break;
            }
            case ']': {
                this->state = this->state_stack.top();
                current = this->state;
                this->state_stack.pop();
                break;
            }
            default:
                break;
        }
    }
}

void Interpreter::reset_interpreter(glm::vec3 position) {
    this->state.position = position;
    this->state.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    this->state.forward = glm::vec3(0.0f, 1.0f, 0.0f);
    this->state.up = glm::vec3(0.0f, 0.0f,1.0f);
    this->state.right = glm::vec3(1.0f, 0.0f, 0.0f);
    this->state.scale_matrix = glm::mat4(1.0f);
    this->state.step = init_length;
    this->state.radius = init_radius;
}