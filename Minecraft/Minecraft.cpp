#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

int main()
{
    Window::init(WINDOW_X, WINDOW_Y, "Minecraft");

    

    while (!Window::isShouldClose())
    {
        glfwPollEvents();
        Window::swapBuffers();
    }
    Window::terminate();
    return 0;
}