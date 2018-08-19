#ifndef _CK_TEXTURE_H
#define _CK_TEXTURE_H

// OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct MyTexture {
    char *buffer;
    unsigned int w, h, sz;
    GLuint tex_id;
    bool valid;

    MyTexture(void);
    void bind(void);
    virtual ~MyTexture(void);

};

struct MyBMPTexture : MyTexture {
    MyBMPTexture(const char *filename);
};

struct MyDDSTexture : MyTexture {
    unsigned int linear_size;
    unsigned int mipmap_count;
    unsigned int fourCC;
    unsigned int components;
    unsigned int format;
    unsigned int block_size;

    MyDDSTexture(const char *filename);
};


#endif /* _CK_TEXTURE_H */
