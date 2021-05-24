#include "Camera.h"
#include "GUI.h"
#include "Graphics/GLWrappers.h"
#include "Maths.h"
#include "Utility.h"
#include <SFML/Graphics.hpp>
#include <cute_headers/cute_sound.h>
#include <stdbool.h>

int main(void)
{
    sf::Window window;
    if (!initWindow(&window)) {
        return 1;
    }
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glViewport(0, 0, WIDTH, HEIGHT);
    // guiInit(window);

    // Init OpenGL Objects
    // clang-format off
    struct Vertex vertices[4] = {

        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f}},
    };
    const GLuint indices[]  = {
        0, 1, 2, 2, 3, 0
    };
    // clang-format on
    struct VertexArray quad = CREATE_VERTEX_ARRAY(vertices, indices);
    struct VertexArray screen = createEmptyVertexArray();

    GLuint shader = loadShaders("MinVertex.glsl", "MinFragment.glsl");
    GLuint frameBufferShader = loadShaders("FramebufferVertex.glsl", "FramebufferFragment.glsl");
    GLuint texture = loadTexture("opengl_logo.png");

    struct Framebuffer framebuffer = createFramebuffer(WIDTH, HEIGHT);

    // Init scene
    struct Camera camera = createCamera();
    camera.position.x = 50;
    camera.position.y = 50;
    camera.position.z = 50;

    float aspect = (float)WIDTH / (float)HEIGHT;

    // Scene objects
    glm::vec3 modelLocations[100];
    glm::vec3 modelRotations[100];

    for (int i = 0; i < 100; i++) {
        modelLocations[i].x = rand() % 75;
        modelLocations[i].y = rand() % 75;
        modelLocations[i].z = rand() % 75;

        modelRotations[i].x = rand() % 360;
        modelRotations[i].y = rand() % 360;
        modelRotations[i].z = rand() % 360;
    }

    struct VertexArray terrain = createTerrainVertexArray();

    // Init window
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // guiBeginFrame();
        // SDL_Event event;
        // while (SDL_PollEvent(&event)) {
        //    guiProcessEvent(&event);
        //    if (event.type == SDL_KEYUP) {
        //        if (event.key.keysym.sym == SDLK_ESCAPE) {
        //            running = false;
        //        }
        //    }
        //    else if (event.type == SDL_MOUSEMOTION) {
        //        cameraMouseInput(&camera, event.motion.xrel, event.motion.yrel);
        //    }
        //    else if (event.type == SDL_QUIT) {
        //        running = false;
        //    }
        //}

        //  Input
        cameraKeyboardInput(&camera);

        // Update
        glm::mat4 projectionViewMatrix{1.0f};
        cameraUpdate(&camera, projectionViewMatrix);

        // Render
        //   guiDebugScreen(&camera);
        glEnable(GL_DEPTH_TEST);

        // 1. Bind framebuffer target
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the scene shaders
        glUseProgram(shader);
        loadMatrix4ToShader(shader, "projectionViewMatrix", projectionViewMatrix);

        // Render the quads
        glBindVertexArray(quad.vao);
        glBindTextureUnit(0, texture);
        for (int i = 0; i < 100; i++) {
            glm::mat4 modelMatrix = createModelMatrix(modelLocations[i], modelRotations[i]);
            loadMatrix4ToShader(shader, "modelMatrix", modelMatrix);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // Render the terrain
        glBindVertexArray(terrain.vao);
        glm::mat4 modelMatrix = createModelMatrix({0, 0, 0}, {0, 0, 0});
        loadMatrix4ToShader(shader, "modelMatrix", modelMatrix);
        glDrawElements(GL_TRIANGLES, terrain.numIndices, GL_UNSIGNED_INT, 0);

        // 2. Unbind framebuffers, render to the window
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(frameBufferShader);
        glBindTextureUnit(0, framebuffer.colourAttachment);

        // Render to window
        glBindVertexArray(screen.vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.display();

        // End frame
        //  guiEndFrame();
        // SDL_GL_SwapWindow(window);
    }

    //=======================================
    //          CLEAN UP
    //=======================================
    destroyVertexArray(&quad);
    destroyVertexArray(&terrain);
    destroyVertexArray(&screen);

    // OpenGL
    glDeleteProgram(shader);
    glDeleteProgram(frameBufferShader);

    // Nuklear
    //    guiShutdown();
    // SDL
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    return 0;
}
