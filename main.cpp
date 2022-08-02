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
using namespace glm;

#include "shader.hpp"
#include "loadTexture.hpp"


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

    int width = 780;
    int height = 640;
    GLFWwindow* window;
    window = glfwCreateWindow(width, height, "footracer", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create glfw window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

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

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.f); 
    glm::mat4 View = glm::lookAt(glm::vec3(2, 3, 6), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, 1.0f));
    Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 mvp = Projection * View * Model;

    static const GLfloat g_vertex_buffer_data[6*6*3] = {
       +1.0f,+1.0f,+1.0f, -1.0f,-1.0f,+1.0f, -1.0f,+1.0f,+1.0f,// front face
       +1.0f,+1.0f,+1.0f, +1.0f,-1.0f,+1.0f, -1.0f,-1.0f,+1.0f,
       -1.0f,-1.0f,-1.0f, -1.0f,+1.0f,-1.0f, +1.0f,+1.0f,-1.0f,// back face
       -1.0f,-1.0f,-1.0f, +1.0f,-1.0f,-1.0f, +1.0f,+1.0f,-1.0f,
       +1.0f,+1.0f,+1.0f, -1.0f,+1.0f,+1.0f, +1.0f,+1.0f,-1.0f,// top face
       -1.0f,+1.0f,-1.0f, -1.0f,+1.0f,+1.0f, +1.0f,+1.0f,-1.0f,
       +1.0f,-1.0f,+1.0f, -1.0f,-1.0f,+1.0f, +1.0f,-1.0f,-1.0f,// bottom face
       -1.0f,-1.0f,-1.0f, -1.0f,-1.0f,+1.0f, +1.0f,-1.0f,-1.0f,
       +1.0f,+1.0f,+1.0f, +1.0f,-1.0f,+1.0f, +1.0f,+1.0f,-1.0f,// right face
       +1.0f,-1.0f,-1.0f, +1.0f,-1.0f,+1.0f, +1.0f,+1.0f,-1.0f,
       -1.0f,+1.0f,+1.0f, -1.0f,-1.0f,+1.0f, -1.0f,+1.0f,-1.0f,// left face
       -1.0f,-1.0f,-1.0f, -1.0f,-1.0f,+1.0f, -1.0f,+1.0f,-1.0f,
    };

    static GLfloat g_color_buffer_data[6*6*3] = {
        0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 1.0f,0.0f,0.0f,//
        0.0f,0.0f,1.0f, 0.5f,1.0f,0.0f, 0.0f,0.0f,1.0f,
        0.0f,1.0f,0.5f, 0.5f,0.0f,1.0f, 0.0f,0.5f,1.0f,//
        0.0f,1.0f,0.5f, 1.0f,0.0f,0.5f, 0.0f,0.5f,1.0f,
        0.0f,0.0f,1.0f, 1.0f,0.0f,0.0f, 0.0f,0.5f,1.0f,//
        0.5f,0.0f,1.0f, 1.0f,0.0f,0.0f, 0.0f,0.5f,1.0f,
        0.5f,1.0f,0.0f, 0.0f,0.0f,1.0f, 1.0f,0.0f,0.5f,//
        0.0f,1.0f,0.5f, 0.0f,0.0f,1.0f, 1.0f,0.0f,0.5f,
        0.0f,0.0f,1.0f, 0.5f,1.0f,0.0f, 0.0f,0.5f,1.0f,//
        1.0f,0.0f,0.5f, 0.5f,1.0f,0.0f, 0.0f,0.5f,1.0f,
        1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f, 0.5f,0.0f,1.0f,//
        0.0f,1.0f,0.5f, 0.0f,0.0f,1.0f, 0.5f,0.0f,1.0f,
    };

    static GLfloat g_uv_buffer_data[6*6*2] = {
        0.630787f,0.360658f, 0.968082f,0.697951f, 0.968082f,0.360658f,
        0.763191f,0.381100f, 0.385350f,0.003256f, 0.385350f,0.381100f,
        0.393932f,0.728072f, 0.072078f,0.728072f, 0.072078f,0.955037f,
        0.072078f,0.955037f, 0.393932f,0.955037f, 0.393932f,0.728072f,
        0.007506f,0.381100f, 0.385350f,0.381100f, 0.385350f,0.003256f,
        0.072078f,0.501107f, 0.393932f,0.728072f, 0.393932f,0.501107f,
        0.465324f,0.740258f, 0.702010f,0.976941f, 0.702010f,0.740258f,
        0.072078f,0.728072f, 0.393932f,0.728072f, 0.072078f,0.501107f,
        0.763193f,0.003256f, 0.385350f,0.003256f, 0.763191f,0.381100f,
        0.465324f,0.976941f, 0.702010f,0.976941f, 0.465324f,0.740258f,
        0.385350f,0.003256f, 0.007506f,0.003256f, 0.007506f,0.381100f,
        0.630787f,0.697951f, 0.968082f,0.697951f, 0.630787f,0.360658f,
    };

    for (int i=0; i<6*6; i++) {
        g_color_buffer_data[3*i + 0] = (g_vertex_buffer_data[3*i + 0] + 1.0f) / 2.0f;
        g_color_buffer_data[3*i + 1] = (g_vertex_buffer_data[3*i + 1] + 1.0f) / 2.0f;
        g_color_buffer_data[3*i + 2] = (g_vertex_buffer_data[3*i + 2] + 1.0f) / 2.0f;
    }

    GLuint Texture = loadBMP_custom("uvtemplate.bmp");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Draw
        // First attribute is vertices array
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // Second attribute is base color
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // Third attribute is a BMP texture
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 6*6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) &&
           (glfwWindowShouldClose(window) == 0));

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}
