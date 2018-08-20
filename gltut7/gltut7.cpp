/*
 * From: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

// OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// OpenGL Math headers (for GLSL)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cube_array.h"
#include "texture.h"
#include "controls.h"
#include "model.h"

using namespace glm;

/* External declaration. */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

mat4
genMVP(mat4 Projection, mat4 View) {
    mat4 Model = mat4(0.1f);
    mat4 mvp = Projection * View * Model;
    return mvp;
}

int
main(int argc, char** argv) {
    /* Some house-keeping, organize variables here. */
    GLFWwindow *appwin = NULL;
    mat4 my_mvp;

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
    appwin = glfwCreateWindow(1152, 864, "Tutorial 7", NULL, NULL);
    if(appwin == NULL) {
        cerr << "Couldn't create OpenGL Window, exiting" << endl;
        glfwTerminate();
        return -1;
    }

    Controls ctrl(appwin);
    ObjModel wonky("gltut7.obj");

    // Set the newly created window to be the current GL context
    glfwMakeContextCurrent(appwin);
    glewExperimental = true;
    if(glewInit() != GLEW_OK) {
        cerr << "Couldn't init libGLEW" << endl;
        glfwTerminate();
        return -1;
    }

    /* Enable Z-test, and use the "LESS" algorithm. */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    wonky.init();

    // This has to do with how keys are interpreted, and allowing ESC
    glfwSetInputMode(appwin, GLFW_STICKY_KEYS, GL_TRUE);

    GLfloat dynamic_color_buffer[12*3*3];

    // Load the Bitmap into a texture
    //MyTexture *tex1 = new MyBMPTexture("../gltut6/tex1.bmp");
    MyTexture *tex1 = new MyDDSTexture("../gltut6/tex1.dds");
    cout << "Tex1: " << "Sz(" << tex1->sz << ")" << endl;

    wonky.colorize();

    // Main event loop
    do {
        ctrl.updateControls();
        /* Beginning of frame work. */
        // Clear the screen every frame
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        my_mvp = genMVP(ctrl.getProjection(), ctrl.getView()); // Generate MVP transformation

        tex1->bind();
        wonky.place();
        wonky.render(my_mvp);

        /* End of frame work */
        // Swap Buffers
        glfwSwapBuffers(appwin);
        // Read event states
        glfwPollEvents();
    } while(glfwGetKey(appwin, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(appwin) == 0);

    // All good!
    return 0;
}

