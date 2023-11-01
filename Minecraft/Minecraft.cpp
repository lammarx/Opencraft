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
#include "ChunkEngine.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

float vertices[] = {
    // x    y
   -0.01f,-0.01f,
    0.01f, 0.01f,

   -0.01f, 0.01f,
    0.01f,-0.01f,
};

int attrs[] = {
        2,  0 //null terminator
};
int drawType;

void config()
{
    drawType = GL_TRIANGLES;
    glewExperimental = GL_TRUE; 
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
    Shader* crosshairShader = load_shader("crosshair.vert", "crosshair.frag");
    Texture* texture = new Texture("tex\\atlas.png");

    ChunkEngine* chunks = new ChunkEngine(8, 1, 8);
    Mesh** meshes = new Mesh*[chunks->volume];
    for (size_t i = 0; i < chunks->volume; i++)
        meshes[i] = nullptr;
    VoxelRenderer renderer(1024 * 1024 * 8);
    
    glClearColor(0.6f, 0.62f, 0.65f, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Mesh* crosshair = new Mesh(vertices, 4, attrs);
    Camera* camera = new Camera(vec3(0, 0, 1), radians(70.0f));
    float cameraSpeed = 5.0f;
    float cameraRotationX = 0.0f;
    float cameraRotationY = 0.0f;

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    std::cout << "Current OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    while (!Window::isShouldClose())
    {
        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        // Debug game control
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
        
        // Movement
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
        
        {
            vec3 end;
            vec3 norm;
            vec3 iend;
            voxel* vox = chunks->raycast(camera->position, camera->front, 10.0f, end, norm, iend);
            if (vox != nullptr) 
            {
                if (Events::justClicked(GLFW_MOUSE_BUTTON_1)) 
                {
                    chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                }
                if (Events::justClicked(GLFW_MOUSE_BUTTON_2))
                {
                    chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 2);
                }
            }
        }

        Chunk* closes[27];
        for (int i = 0; i < chunks->volume; i++)
        {
            Chunk* chunk = chunks->chunks[i];
            if (!chunk->modified)
                continue;
            chunk->modified = false;
            if (meshes[i] != nullptr)
                delete meshes[i];
            for (int i = 0; i < 27; i++)
            {
                closes[i] = nullptr;
            }
            for (size_t j = 0; j < chunks->volume; j++)
            {
                Chunk* other = chunks->chunks[j];
                int ox = other->x - chunk->x;
                int oy = other->y - chunk->y;
                int oz = other->z - chunk->z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                    continue;
                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 + oz) * 3 + ox] = other;
            }
            Mesh* mesh = renderer.render(chunk, (const Chunk**)closes);
            meshes[i] = mesh;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shader->use();
        shader->uniformMatrix("projview", camera->getProjection() * camera->getView());
        texture->bind();
        mat4 model(1.0f);
        for (size_t i = 0; i < chunks->volume; i++) 
        {
            Chunk* chunk = chunks->chunks[i];
            Mesh* mesh = meshes[i];
            model = glm::translate(mat4(1.0f), vec3(chunk->x * CHUNK_W + 0.5f, chunk->y * CHUNK_H + 0.5f, chunk->z * CHUNK_D + 0.5f));
            shader->uniformMatrix("model", model);
            mesh->draw(drawType);
        }
        crosshairShader->use();
        crosshair->draw(GL_LINES);
        Window::swapBuffers();
        Events::pullEvents();
    }

    delete shader;
    delete crosshairShader;
    delete crosshair;
    delete texture;
    delete chunks;
    Window::terminate();
    return 0;
}