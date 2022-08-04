#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() { return ViewMatrix; }
glm::mat4 getProjectionMatrix() { return ProjectionMatrix; }

// Handle inputs
glm::vec3 position = glm::vec3(1, 1, 5);
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFOV = 45.0f;

float speed = 6.0f;
float mouseSpeed = 0.6f;
double xpos, ypos;

void computeMatricesFromInputs(GLFWwindow* window, int width, int height)
{
    // Next line is called only once the first time the function is called
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, (double) width/2, (double) height/2);
    horizontalAngle += mouseSpeed * deltaTime * float(width/2.0f - xpos);
    verticalAngle   += mouseSpeed * deltaTime * float(height/2.0f - ypos);
    glm::vec3 direction (
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );
    glm::vec3 up = glm::cross(right, direction);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }
    ProjectionMatrix = glm::perspective(initialFOV, (float) width/height, 0.1f, 100.f);
    ViewMatrix = glm::lookAt(position, position + direction, up);
    lastTime = currentTime;
}
