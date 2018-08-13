/*
 * From: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

// OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

int
main(int argc, char** argv) {
    // First, initialize GLFW
    glewExperimental = true;
    if(!glfwInit()) {
        cerr << "Failed to init GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Utilize OpenGL 3.3 API, etc...
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use "core" OpenGL, not legacy

    // Now, open a window
    GLFWwindow *appwin = NULL;
    appwin = glfwCreateWindow(1152, 864, "Tutorial 1", NULL, NULL);
    if(appwin == NULL) {
        cerr << "Couldn't create OpenGL Window, exiting" << endl;
        glfwTerminate();
        return -1;
    }

    // Set the newly created window to be the current GL context
    glfwMakeContextCurrent(appwin);
    glewExperimental = true;
    if(glewInit() != GLEW_OK) {
        cerr << "Couldn't init libGLEW" << endl;
        glfwTerminate();
        return -1;
    }

    // This has to do with how keys are interpreted, and allowing ESC
    glfwSetInputMode(appwin, GLFW_STICKY_KEYS, GL_TRUE);

    // Main event loop
    do {
        // Clear the screen every frame
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap Buffers
        glfwSwapBuffers(appwin);

        // Read event states
        glfwPollEvents();
    } while(glfwGetKey(appwin, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(appwin) == 0);

    // All good!
    return 0;
}

