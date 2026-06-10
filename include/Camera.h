#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 2.0f, 5.0f));

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
};