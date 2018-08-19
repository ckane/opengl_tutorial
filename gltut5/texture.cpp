#include "texture.h"

MyTexture::MyTexture(void) : buffer(nullptr), w(0), h(0), sz(0), tex_id(0), valid(false) {
    glGenTextures(1, &(this->tex_id));
};

void
MyTexture::bind(void) {
    glBindTexture(GL_TEXTURE_2D, this->tex_id);
};

MyTexture::~MyTexture(void) {
    if(buffer != nullptr) {
        delete buffer;
    };
};
