/*
 * From: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
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

/* External declaration. */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

int
main(int argc, char** argv) {
    /* Some house-keeping, organize variables here. */
    GLFWwindow *appwin = NULL;
    GLuint vtx_array_id_1 = GL_NONE;
    GLuint vtx_buffer_1 = GL_NONE;
    GLuint glsl_programId = GL_NONE;

    // 3 triples to describe the three points of a triangle in 3d-space
    static const GLfloat triangle_vtx_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, 0.5f,  // Bottom-right
        0.0f, 1.0f, 0.0f    // Top-middle
    };

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

    // Create a new Vertex Array Object
    glGenVertexArrays(1, &vtx_array_id_1); // Initialize a new array id, put this num in vtx_array_id_1
    glBindVertexArray(vtx_array_id_1);     // Bind this new array id to the current context

    // Create new buffer object
    glGenBuffers(1, &vtx_buffer_1); // Initialize a new buffer, store its handle in vtx_buffer_1
    glBindBuffer(GL_ARRAY_BUFFER, vtx_buffer_1); // Bind this new buffer to the current context
    // Give the 3x3 vertex list to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vtx_buffer_data), triangle_vtx_buffer_data, GL_STATIC_DRAW);

    /* Load the shaders into the context. Yielding a GLSL program id. */
    glsl_programId = LoadShaders("tut2.vert", "tut2.frag");


    // This has to do with how keys are interpreted, and allowing ESC
    glfwSetInputMode(appwin, GLFW_STICKY_KEYS, GL_TRUE);

    // Main event loop
    do {
        /* Beginning of frame work. */

        // Clear the screen every frame
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Rendering work. */
        glEnableVertexAttribArray(0); // Enable array #0
        glBindBuffer(GL_ARRAY_BUFFER, vtx_buffer_1);
        glVertexAttribPointer(0, 3 /* size */, GL_FLOAT /* type */, GL_FALSE /* normalized? */,
                              0 /* stride */, (void*)0 /* array buffer offset */);
        /* Use GLSL program. */
        glUseProgram(glsl_programId);

        // Draw triangle (after setting what GLSL program to use)
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle, 3 vertexes = 1 triangle

        glDisableVertexAttribArray(0); // Disable array #0

        /* End of frame work */
        // Swap Buffers
        glfwSwapBuffers(appwin);
        // Read event states
        glfwPollEvents();
    } while(glfwGetKey(appwin, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(appwin) == 0);

    // All good!
    return 0;
}

