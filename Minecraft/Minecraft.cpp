#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Window.h"
#include "Events.h"
#include "Shader.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

float vertices[] = {
        // x    y     z     u     v
       -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
       -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

void config()
{
    
}

int main()
{
    glewExperimental = GL_TRUE;
    glewInit();
    Window::init(WINDOW_X, WINDOW_Y, "Minecraft");
    Events::init();

    glClearColor(0, 0, 0, 1);
    
    Shader* shader = load_shader("main.vert", "main.frag");
    
    if (shader == nullptr) 
    {
        std::cerr << "failed to load shader" << std::endl;
        Window::terminate();
        return 1;
    }
    std::cout << glGetString(GL_VERSION);
    GLuint VAO = 0, VBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glClearColor(0.6f, 0.62f, 0.65f, 1);

    while (!Window::isShouldClose())
    {
        Events::pullEvents();

        if (Events::justPressed(GLFW_KEY_ESCAPE))
        {
            Window::setShouldClose(true);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        Window::swapBuffers();
    }

    delete shader;
    Window::terminate();
    return 0;
}