//
// Created by Niccolo on 28/03/2025.
//

#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->movementSpeed = SPEED;
    this->mouseSensitivity = SENSITIVITY;
    this->zoom = ZOOM;

    this->updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->yaw = yaw;
    this->pitch = pitch;
    this->movementSpeed = SPEED;
    this->mouseSensitivity = SENSITIVITY;
    this->zoom = ZOOM;

    this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    direction.y = sin(glm::radians(this->pitch));
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(direction);

    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    if (direction == FORWARD)
        this->position += this->front * velocity;
    if (direction == BACKWARD)
        this->position -= this->front * velocity;
    if (direction == LEFT)
        this->position -= this->right * velocity;
    if (direction == RIGHT)
        this->position += this->right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (this->pitch > 89.0f) {
        this->pitch = 89.0f;
    }
    if (this->pitch < -89.0f) {
        this->pitch = -89.0f;
    }

    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    this->zoom -= (float)yoffset;
    if (this->zoom < 1.0f)
        this->zoom = 1.0f;
    if (this->zoom > 45.0f)
        this->zoom = 45.0f;
}