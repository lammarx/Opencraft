#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Events.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

int main()
{
    Window::init(WINDOW_X, WINDOW_Y, "Minecraft");
    Events::init();
    while (!Window::isShouldClose())
    {
        Events::pullEvents();

        if (Events::justPressed(GLFW_KEY_ESCAPE))
        {
            Window::setShouldClose(true);
        }

        Window::swapBuffers();
    }
    Window::terminate();
    return 0;
}