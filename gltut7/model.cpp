// OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "model.h"

using namespace glm;
using namespace std;

/* External declaration. */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

ObjModel::ObjModel(const char *filename) : obj_file_name(filename), valid(false) {
    this->load_object();
};

void
ObjModel::load_object(void) {
    vector<unsigned int> vidx, uvidx, nidx;
    vector<vec3> tmp_vtx, tmp_norm;
    vector<vec2> tmp_uv;

    ifstream objf(this->obj_file_name);
    if(objf) {
        do {
            string first_tok;
            objf >> first_tok;
            if(first_tok == "v") {
                vec3 vtx;
                objf >> vtx.x >> vtx.y >> vtx.z;
                tmp_vtx.push_back(vtx);
                //cout << "Vertex: " << "<" << vtx.x << "," << vtx.y << "," << vtx.z << ">" << endl;
            } else if(first_tok == "vt") {
                vec2 uv;
                objf >> uv.x >> uv.y;
                tmp_uv.push_back(uv);
            } else if(first_tok == "vn") {
                vec3 normvec;
                objf >> normvec.x >> normvec.y >> normvec.z;
                tmp_norm.push_back(normvec);
            } else if(first_tok == "f") {
                unsigned int i1, i2, i3;
                char delim;

                /* Read in first field (vertex indices). */
                for(int i = 0; i < 3; i++) {
                    objf >> i1 >> delim >> i2 >> delim >> i3;
                    //cout << i1 << " " << i2 << " " << i3 << endl;
                    vidx.push_back(i1); uvidx.push_back(i2); nidx.push_back(i3);
                }
            } else {
                /* If a line doesn't start with v/vt/vn/f, then skip it */
                char s[256];
                objf.getline(s, 1000);
            }
        } while(objf);

        /* The vertexes and indices are all loaded, now we just need to normalize them. */
        for(unsigned int i = 0; i < vidx.size(); i++) {
            vec3 v = tmp_vtx[vidx[i] - 1];
            ovtx.push_back(v);
        };
        for(unsigned int i = 0; i < uvidx.size(); i++) {
            vec2 uv = tmp_uv[uvidx[i] - 1];
            ouvs.push_back(uv);
        };
        for(unsigned int i = 0; i < nidx.size(); i++) {
            vec3 n = tmp_norm[nidx[i] - 1];
            onorm.push_back(n);
        };

        valid = true;
    };
};

void
ObjModel::init(void) {
    glGenVertexArrays(1, &varr);
    glBindVertexArray(varr);     // Bind this new array id to the current context

    // Create new buffer object, for vertexes
    glGenBuffers(1, &vbuf); // Initialize a new buffer, store its handle in vtx_buffer_1
    glBindBuffer(GL_ARRAY_BUFFER, vbuf); // Bind this new buffer to the current context

    // Give the 3x3 vertex list to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * ovtx.size(), &ovtx[0], GL_STATIC_DRAW);

    /* Load the shaders into the context. Yielding a GLSL program id. */
    progid = LoadShaders("simple.vert", "texture.frag");

    /* Get a UniformLocation Id for the "MVP" object in the GLSL. */
    mvp = glGetUniformLocation(progid, "MVP");
};

void
ObjModel::colorize(void) {
    glGenBuffers(1, &uvbuf);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * ouvs.size(), &ouvs[0], GL_STATIC_DRAW);
};

void
ObjModel::place(void) {
    glEnableVertexAttribArray(0); // Enable array #0
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glVertexAttribPointer(0, 3 /* size */, GL_FLOAT /* type */, GL_FALSE /* normalized? */,
            0 /* stride */, (void*)0 /* array buffer offset */);

    glEnableVertexAttribArray(1); // Enable array #1
    glBindBuffer(GL_ARRAY_BUFFER, uvbuf);
    glVertexAttribPointer(1, 2 /* size */, GL_FLOAT /* type */, GL_FALSE /* normalized? */,
            0 /* stride */, (void*)0 /* array buffer offset */);
};

void
ObjModel::render(const mat4& in_mvp) {
    /* Use GLSL program. */
    glUseProgram(progid);
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &in_mvp[0][0]);

    // Draw triangle (after setting what GLSL program to use)
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // Draw the triangle, 12 vertexes = 6 triangles

    glDisableVertexAttribArray(1); // Disable array #1
    glDisableVertexAttribArray(0); // Disable array #0
};
