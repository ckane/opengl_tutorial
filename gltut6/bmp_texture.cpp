#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "texture.h"

using namespace std;

MyBMPTexture::MyBMPTexture(const char *filename) : MyTexture() {
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
