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


int drawType;

void config()
{
    drawType = GL_TRIANGLES;
    glewExperimental = GL_TRUE; 

    std::cout << "Current OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

int main()
{
    config();
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
    Texture* texture = new Texture("tex\\grass.png");

    VoxelRenderer renderer(1024 * 1024 * 8);
    Chunk* chunk = new Chunk();
    Mesh* mesh = renderer.render(chunk);

    glClearColor(0.6f, 0.62f, 0.65f, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    Camera* camera = new Camera(vec3(0, 0, 1), radians(70.0f));

    glm::mat4 model(1.0f);
    model = translate(model, vec3(0.5f, 0, 0));

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float cameraSpeed = 5.0f;
    const float cameraMaxSpeed = 5.0f;
    const float cameraAcceleration = 0.0f;
    float cameraRotationX = 0.0f;
    float cameraRotationY = 0.0f;

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
        if (Events::justPressed(GLFW_KEY_Q)) {
            if (drawType == GL_TRIANGLES) drawType = GL_LINES;
            else { drawType = GL_TRIANGLES; }
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
            cameraRotationX += -Events::dy / Window::height;
            cameraRotationY += -Events::dx / Window::height;
            cameraRotationX = clamp(cameraRotationX, radians(-90.0f), radians(90.0f));
            camera->rotation = mat4(1.0f);
            camera->rotate(cameraRotationX, cameraRotationY, 0);
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix("projview", camera->getProjection() * camera->getView());
        texture->bind();
        mesh->draw(drawType);
        
        Window::swapBuffers();
        Events::pullEvents();
    }

    delete shader;
    delete mesh;
    delete chunk;
    delete texture;

    Window::terminate();
    return 0;
}