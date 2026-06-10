#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position) {
    Position = position;
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = -90.0f;
    Pitch = 0.0f;
    MovementSpeed = 5.0f;
    MouseSensitivity = 0.1f;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == 0) Position += Front * velocity;      // FORWARD
    if (direction == 1) Position -= Front * velocity;      // BACKWARD
    if (direction == 2) Position -= Right * velocity;      // LEFT
    if (direction == 3) Position += Right * velocity;      // RIGHT
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    
    Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
}