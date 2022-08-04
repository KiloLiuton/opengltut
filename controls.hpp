#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void computeMatricesFromInputs(GLFWwindow* window, int width, int height);

#endif
