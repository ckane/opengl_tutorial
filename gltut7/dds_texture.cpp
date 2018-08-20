#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "texture.h"
#include "fourcc_codes.h"

using namespace std;

MyDDSTexture::MyDDSTexture(const char *filename) : MyTexture() {
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
