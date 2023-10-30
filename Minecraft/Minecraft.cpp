#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "VoxelRenderer.h"
#include "Window.h"
#include "Events.h"
#include "Camera.h"
#include "voxel.h"
#include "Chunk.h"


#define WINDOW_X 1280
#define WINDOW_Y 720


void config()
{
    glewExperimental = GL_TRUE; 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main()
{
    config();
    glewInit();
    Window::init(WINDOW_X, WINDOW_Y, "Minecraft");
    Events::init();

    glClearColor(0, 0, 0, 1);
    const float cameraSpeed = 5.0f;
    Shader* shader = load_shader("main.vert", "main.frag");
    
    if (shader == nullptr) 
    {
        std::cerr << "failed to load shader" << std::endl;
        Window::terminate();
        return 1;
    }
    std::cout << "Current OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Texture texture("tex\\grass.bmp");

    VoxelRenderer renderer(1024 * 2048 * 8);
    Chunk* chunk = new Chunk();
    Mesh* mesh = renderer.render(chunk);

    glClearColor(0.6f, 0.62f, 0.65f, 1);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Camera* camera = new Camera(vec3(0, 0, 1), radians(70.0f));

    glm::mat4 model(1.0f);

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float camx = 0.0f;
    float camy = 0.0f;

    while (!Window::isShouldClose())
    {
        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        if (Events::justPressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }
        if (Events::justPressed(GLFW_KEY_TAB)) {
            Events::toggleCursor();
        }
        if (Events::pressed(GLFW_KEY_W)) {
            camera->position += camera->front * delta * cameraSpeed;
        }
        if (Events::pressed(GLFW_KEY_S)) {
            camera->position -= camera->front * delta * cameraSpeed;
        }
        if (Events::pressed(GLFW_KEY_A)) {
            camera->position -= camera->right * delta * cameraSpeed;
        }
        if (Events::pressed(GLFW_KEY_D)) {
            camera->position += camera->right * delta * cameraSpeed;
        }
        if (Events::_cursor_locked) {
            camx += -Events::dy / Window::height;
            camy += -Events::dx / Window::height;
            camy = clamp(camy, -90.0f, 90.0f);
            camera->rotation = mat4(1.0f);
            camera->rotate(camx, camy, 0);
        }
        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix("projview", camera->getProjection() * camera->getView());

        mesh->draw(GL_TRIANGLES);
        Window::swapBuffers();
        Events::pullEvents();
    }

    delete shader;
    delete mesh;
    delete chunk;
    Window::terminate();
    return 0;
}