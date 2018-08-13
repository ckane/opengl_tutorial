/*
 * From: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

// OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// OpenGL Math headers (for GLSL)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

/* External declaration. */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

mat4
genMVP(float width, float height) {
    mat4 Projection = perspective(radians(45.f), (float)width / (float)height, 0.1f, 100.0f);
    //mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates
    mat4 View = lookAt(vec3(4,3,3), vec3(0,0,0), vec3(0,1,0));
    mat4 Model = mat4(0.1f);
    mat4 mvp = Projection * View * Model;
    return mvp;
}

struct MyModel {
    GLuint vtx_array_id;
    GLuint vtx_buffer;
    GLuint programId;
    GLuint mvp_matrix_id;
    GLfloat *bufdata;
    size_t buflen;

    MyModel() : vtx_array_id(GL_NONE), vtx_buffer(GL_NONE), programId(GL_NONE), bufdata(nullptr), buflen(0) {};

    void init(const GLfloat *buffer_data, size_t buffer_data_len) {
        // Create a new Vertex Array Object
        glGenVertexArrays(1, &vtx_array_id); // Initialize a new array id, put this num in vtx_array_id_1
        glBindVertexArray(vtx_array_id);     // Bind this new array id to the current context

        // Create new buffer object
        glGenBuffers(1, &vtx_buffer); // Initialize a new buffer, store its handle in vtx_buffer_1
        glBindBuffer(GL_ARRAY_BUFFER, vtx_buffer); // Bind this new buffer to the current context

        // Make a local copy of the buffer data
        bufdata = new GLfloat[buffer_data_len];

        memcpy(bufdata, buffer_data, buffer_data_len * sizeof(*buffer_data));
        buflen = buffer_data_len;

        // Give the 3x3 vertex list to OpenGL
        glBufferData(GL_ARRAY_BUFFER, sizeof(*bufdata) * buflen, bufdata,
                     GL_STATIC_DRAW);

        /* Load the shaders into the context. Yielding a GLSL program id. */
        programId = LoadShaders("simple.vert", "tut3.frag");

        /* Get a UniformLocation Id for the "MVP" object in the GLSL. */
        mvp_matrix_id = glGetUniformLocation(programId, "MVP");
    };

    void place(void) {
        glBindBuffer(GL_ARRAY_BUFFER, vtx_buffer);
        glVertexAttribPointer(0, 3 /* size */, GL_FLOAT /* type */, GL_FALSE /* normalized? */,
                              0 /* stride */, (void*)0 /* array buffer offset */);
    };

    void render(const mat4& mvp) {
        /* Use GLSL program. */
        glUseProgram(programId);
        glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, &mvp[0][0]);

        // Draw triangle (after setting what GLSL program to use)
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle, 3 vertexes = 1 triangle
    };
};

int
main(int argc, char** argv) {
    /* Some house-keeping, organize variables here. */
    GLFWwindow *appwin = NULL;
    MyModel triangles;
    mat4 my_mvp;

    // 3 triples to describe the three points of a triangle in 3d-space
    static const GLfloat triangle_vtx_buffer_data[] = {
        /* Tri 1 */
        -1.0f, -1.0f, 0.0f, // Bottom-left
        1.0f, -1.0f, 0.0f,  // Bottom-right
        0.0f, 1.0f, 0.0f,    // Top-middle
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

    triangles.init(&triangle_vtx_buffer_data[0], sizeof(triangle_vtx_buffer_data)/sizeof(triangle_vtx_buffer_data[0]));

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

        /* Generate the MVP xform matrix for the current view. */
        my_mvp = genMVP(1152.f, 864.f); // Generate MVP transformation


        /* Draw my "triangles" model. */
        triangles.place();
        triangles.render(my_mvp);

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

