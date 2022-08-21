#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "loadTexture.hpp"
#include "controls.hpp"
#include "objloader.hpp"


int main(int argc, char** argv)
{
    if (!glfwInit()) {
        fprintf(stderr, "\n");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    int width = 1280;
    int height = 720;
    GLFWwindow* window;
    window = glfwCreateWindow(width, height, "footracer", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create glfw window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW!\n");
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint LightID = glGetUniformLocation(programID, "LightPosition");
    GLuint LightPowerID = glGetUniformLocation(programID, "LightPower");

    // Set texture
    //GLuint Texture = loadBMP_custom("blender_assets/cube_uv.bmp");
    GLuint Texture = loadBMP_custom("blender_assets/macaco.bmp");
    //GLuint Texture = loadDDS("uvtemplate.DDS");

    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // Load obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    //loadOBJ("blender_assets/cube_uv.obj", vertices, uvs, normals);
    loadOBJ("blender_assets/macaco.obj", vertices, uvs, normals);
    //GLMmodel* model = glmReadOBJ("blender_assets/cube_uv.obj");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint texturebuffer;
    glGenBuffers(1, &texturebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    //glEnable(GL_CULL_FACE);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        computeMatricesFromInputs(window, width, height);
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glm::vec3 light = {2.0f, 1.0f, 0.0f};
        float lightpower = 1.0f;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        glUniformMatrix4fv(LightID, 1, GL_FALSE, &light[0]);
        glUniformMatrix4fv(LightPowerID, 1, GL_FALSE, &lightpower);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0);

        // Draw
        // First attribute is vertices array
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Second attribute is a BMP texture
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Third attribute is normal buffer
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) &&
           (glfwWindowShouldClose(window) == 0));

    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &texturebuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}
