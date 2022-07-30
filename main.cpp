#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main(int argc, char** argv)
{
  if (!glfwInit()) {
    fprintf(stderr, "\n");
    return -1;
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow* window;
  window = glfwCreateWindow(780, 640, "footracer", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create glfw window!\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW!\n");
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  do {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

  return 0;
}
