//
// Created by Niccolo on 29/04/2025.
//

#include "interpreter.h"

#include <utility>

Interpreter::Interpreter(std::shared_ptr<Branch> branch, std::shared_ptr<Leaf> leaf, float angle, float radius, float length, float radius_decay, float length_decay) : radius(radius), length(length), radius_decay(radius_decay), length_decay(length_decay) {
    this->builder_map['F'] = branch;
    this->builder_map['P'] = branch;
    this->builder_map['L'] = leaf;

    TurtleState s = TurtleState();
    s.position = glm::vec3(0.0f);
    s.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    s.forward = glm::vec3(0.0f, 1.0f, 0.0f);
    s.up = glm::vec3(0.0f, 0.0f,1.0f);
    s.right = glm::vec3(1.0f, 0.0f, 0.0f);
    this->movement_step = this->length;
    this->angle = glm::radians(angle);
    this->state = s;
}

void Interpreter::read_string(const std::string &predicate, std::vector<Mesh>& meshes, std::vector<glm::mat4>& transforms) {
    TurtleState current = this->state;
    glm::quat rot;
    glm::mat4 translation, rotation;
    for (char c : predicate) {
        switch (c) {
            case 'P': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation);
                this->builder_map['P']->build_branch(0.5f * this->length, this->radius, 0);
                meshes.push_back(this->builder_map['F']->getResult());
                break;
            }
            case 'F': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation);
                this->builder_map['F']->build_branch(this->length, this->radius, this->radius);
                glm::vec3 movement_direction = glm::normalize(glm::vec3(current.forward.x, current.forward.y, current.forward.z));
                this->state.position = current.position + this->movement_step * movement_direction;
                current = this->state;
                meshes.push_back(this->builder_map['F']->getResult());
                break;
            }
            case 'L': {
                translation = glm::translate(glm::mat4(1.0f), current.position);
                rotation = glm::mat4_cast(current.orientation);
                transforms.push_back(translation * rotation);
                this->builder_map['L']->build_leaf();
                meshes.push_back(this->builder_map['L']->getResult());
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
            case '\\': {
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