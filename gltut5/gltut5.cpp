/*
 * From: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
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
#include "fourcc_codes.h"

using namespace glm;

/* External declaration. */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

mat4
genMVP(float width, float height) {
    mat4 Projection = perspective(radians(45.f), (float)width / (float)height, 0.1f, 100.0f);
    //mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates
    mat4 View = lookAt(vec3(4,3,-3), vec3(0,0,0), vec3(0,1,0));
    mat4 Model = mat4(0.1f);
    mat4 mvp = Projection * View * Model;
    return mvp;
}

struct MyTexture {
    char *buffer;
    unsigned int w, h, sz;
    GLuint tex_id;
    bool valid;

    MyTexture(void) : buffer(nullptr), w(0), h(0), sz(0), tex_id(0), valid(false) {
        glGenTextures(1, &(this->tex_id));
    };

    void bind(void) {
        glBindTexture(GL_TEXTURE_2D, this->tex_id);
    };

    virtual ~MyTexture(void) {
        if(buffer != nullptr) {
            delete buffer;
        };
    };
};

struct MyBMPTexture : MyTexture {
    MyBMPTexture(const char *filename) : MyTexture() {
        char header_buffer[54];
        unsigned int data_pos;

        ifstream bmpfile(filename);
        bmpfile.read(header_buffer, sizeof(header_buffer));
        data_pos = *(int*)&(header_buffer[0x0a]);
        this->sz = *(int*)&(header_buffer[0x22]);
        this->w  = *(int*)&(header_buffer[0x12]);
        this->h  = *(int*)&(header_buffer[0x16]);

        if(this->sz == 0) {
            this->sz = this->w * this->h * 3;
        }

        if(data_pos == 0) {
            data_pos = 54;
        }

        /* Next allocate the dynamic buffer for the raster data. */
        this->buffer = new char[this->sz];

        /* Finally, seek to the image data position, and read the pixels into RAM. */
        bmpfile.seekg(data_pos);
        bmpfile.read(this->buffer, this->sz);

        /* Next allocate new texture for this, and bind it so we can set it up. */
        this->bind();

        /* Assign the raster data to this texture id. */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->w, this->h, 0, GL_BGR, GL_UNSIGNED_BYTE, this->buffer);

        /* Set texture-mapping policy. */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        /* Generate MipMap */
        glGenerateMipmap(GL_TEXTURE_2D);

        valid = true;
    };
};

struct MyDDSTexture : MyTexture {
    unsigned int linear_size;
    unsigned int mipmap_count;
    unsigned int fourCC;
    unsigned int components;
    unsigned int format;
    unsigned int block_size;

    MyDDSTexture(const char *filename) : MyTexture() {
        unsigned char header[124];
        char filecode[4];

        ifstream ddsfile(filename);
        ddsfile.read(&filecode[0], sizeof(filecode));
        if(strncmp(filecode, "DDS ", 4) == 0) {
            ddsfile.read((char*)&header[0], sizeof(header));
            this->h = *(unsigned int*)&(header[8]);
            this->w = *(unsigned int*)&(header[12]);
            this->linear_size = *(unsigned int*)&(header[16]);
            this->mipmap_count = *(unsigned int*)&(header[24]);
            this->fourCC = *(unsigned int*)&(header[80]);

            if(this->mipmap_count > 1) {
                /* If we have mipmaps, the buffer will be 2x image size. */
                this->sz = this->linear_size * 2;
            } else {
                /* Otherwise, buffer_sz = linear_size */
                this->sz = this->linear_size;
            };

            this->buffer = new char[this->sz];
            ddsfile.read(this->buffer, this->sz);

            if(this->fourCC == FOURCC_DXT1) {
                this->components = 3;
            } else {
                this->components = 4;
            };

            switch(this->fourCC) {
                case FOURCC_DXT1:
                    this->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                    this->block_size = 8;
                    break;
                case FOURCC_DXT3:
                    this->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                    this->block_size = 16;
                    break;
                case FOURCC_DXT5:
                    this->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                    this->block_size = 16;
                    break;
            };

            cout << "FourCC Code: " << (char)(this->fourCC & 0xff);
            cout << (char)(this->fourCC >> 8 & 0xff);
            cout << (char)(this->fourCC >> 16 & 0xff);
            cout << (char)(this->fourCC >> 24 & 0xff) << endl;

            this->bind();

            /* Load mipmaps. */
            unsigned int offset = 0;
            unsigned l_width = this->w;
            unsigned l_height = this->h;

            cout << "MipMap Levels: " << this->mipmap_count << endl;
            for(unsigned int mm_level = 0; mm_level < this->mipmap_count && (l_width || l_height); mm_level++) {
                unsigned int size = ((l_width+3)/4)*((l_height+3)/4)*this->block_size;
                glCompressedTexImage2D(GL_TEXTURE_2D, mm_level, this->format, l_width, l_height, 0,
                                       size, this->buffer + (uintptr_t)offset);
                offset += size;
                l_width >>= 1;
                l_height >>= 1;
            };
            this->valid = true;
        };
    };
};

struct MyModel {
    GLuint vtx_array_id;
    GLuint vtx_buffer;
    GLuint programId;
    GLuint mvp_matrix_id;
    GLuint color_buffer;
    GLfloat *bufdata, *colordata;
    size_t buflen, color_buflen;

    MyModel() : vtx_array_id(GL_NONE), vtx_buffer(GL_NONE), programId(GL_NONE), bufdata(nullptr), buflen(0), color_buffer(GL_NONE),
                colordata(nullptr), color_buflen(0) {};

    void init(const GLfloat *buffer_data, size_t buffer_data_len) {
        // Create a new Vertex Array Object
        glGenVertexArrays(1, &vtx_array_id); // Initialize a new array id, put this num in vtx_array_id_1
        glBindVertexArray(vtx_array_id);     // Bind this new array id to the current context

        // Create new buffer object, for vertexes
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
        programId = LoadShaders("simple.vert", "tut5.frag");

        /* Get a UniformLocation Id for the "MVP" object in the GLSL. */
        mvp_matrix_id = glGetUniformLocation(programId, "MVP");
    };

    void colorize(const GLfloat *color_buffer_data, size_t color_buffer_data_len) {
        glGenBuffers(1, &color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        colordata = new GLfloat[color_buffer_data_len];
        color_buflen = color_buffer_data_len;
        memcpy(colordata, color_buffer_data, color_buffer_data_len * sizeof(*color_buffer_data));
        glBufferData(GL_ARRAY_BUFFER, sizeof(*colordata) * color_buflen, colordata, GL_STATIC_DRAW);
    };

    void place(void) {
        glEnableVertexAttribArray(0); // Enable array #0
        glBindBuffer(GL_ARRAY_BUFFER, vtx_buffer);
        glVertexAttribPointer(0, 3 /* size */, GL_FLOAT /* type */, GL_FALSE /* normalized? */,
                              0 /* stride */, (void*)0 /* array buffer offset */);

        glEnableVertexAttribArray(1); // Enable array #0
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glVertexAttribPointer(1, 2 /* size */, GL_FLOAT /* type */, GL_FALSE /* normalized? */,
                              0 /* stride */, (void*)0 /* array buffer offset */);
    };

    void render(const mat4& mvp) {
        /* Use GLSL program. */
        glUseProgram(programId);
        glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, &mvp[0][0]);

        // Draw triangle (after setting what GLSL program to use)
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Draw the triangle, 12 vertexes = 6 triangles

        glDisableVertexAttribArray(1); // Disable array #1
        glDisableVertexAttribArray(0); // Disable array #0
    };
};

int
main(int argc, char** argv) {
    /* Some house-keeping, organize variables here. */
    GLFWwindow *appwin = NULL;
    MyModel triangles;
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
    appwin = glfwCreateWindow(1152, 864, "Tutorial 5", NULL, NULL);
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

    /* Enable Z-test, and use the "LESS" algorithm. */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    triangles.init(&cube_vertex_buffer_array[0], sizeof(cube_vertex_buffer_array)/sizeof(cube_vertex_buffer_array[0]));

    // This has to do with how keys are interpreted, and allowing ESC
    glfwSetInputMode(appwin, GLFW_STICKY_KEYS, GL_TRUE);

    GLfloat dynamic_color_buffer[12*3*3];

    // Load the Bitmap into a texture
    //MyTexture *tex1 = new MyBMPTexture("tex1.bmp");
    MyTexture *tex1 = new MyDDSTexture("tex1.dds");
    cout << "Tex1: " << "Sz(" << tex1->sz << ")" << endl;

    triangles.colorize(uv_buffer_data, sizeof(uv_buffer_data)/sizeof(uv_buffer_data[0]));

    // Main event loop
    do {
        /* Beginning of frame work. */
        // Clear the screen every frame
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Rendering work. */
        /* Generate the MVP xform matrix for the current view. */
        my_mvp = genMVP(1152.f, 864.f); // Generate MVP transformation

        /* Draw my "triangles" model. */
        tex1->bind();
        triangles.place();
        triangles.render(my_mvp);

        /* End of frame work */
        // Swap Buffers
        glfwSwapBuffers(appwin);
        // Read event states
        glfwPollEvents();
    } while(glfwGetKey(appwin, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(appwin) == 0);

    // All good!
    return 0;
}

