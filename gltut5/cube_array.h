#include <GL/gl.h>

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat cube_vertex_buffer_array[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
static const GLfloat uv_buffer_data[] = {
    0.000000f, 1.0f-0.000000f,
    0.000000f, 1.0f-0.500000f,
    0.333333f, 1.0f-0.500000f,
    1.000000f, 1.0f-0.000000f,
    0.666667f, 1.0f-0.500000f,
    1.000000f, 1.0f-0.500000f,
    0.666667f, 1.0f-0.500000f,
    0.333333f, 1.0f-1.000000f,
    0.666667f, 1.0f-1.000000f,
    1.000000f, 1.0f-0.000000f,
    0.666667f, 1.0f-0.000000f,
    0.666667f, 1.0f-0.500000f,
    0.000000f, 1.0f-0.000000f,
    0.333333f, 1.0f-0.500000f,
    0.333333f, 1.0f-0.000000f,
    0.666667f, 1.0f-0.500000f,
    0.333333f, 1.0f-0.500000f,
    0.333333f, 1.0f-1.000000f,
    1.000000f, 1.0f-1.000000f,
    1.000000f, 1.0f-0.500000f,
    0.666667f, 1.0f-0.500000f,
    0.666667f, 1.0f-0.000000f,
    0.333333f, 1.0f-0.500000f,
    0.666667f, 1.0f-0.500000f,
    0.333333f, 1.0f-0.500000f,
    0.666667f, 1.0f-0.000000f,
    0.333333f, 1.0f-0.000000f,
    0.000000f, 1.0f-0.500000f,
    0.000000f, 1.0f-1.000000f,
    0.333333f, 1.0f-1.000000f,
    0.000000f, 1.0f-0.500000f,
    0.333333f, 1.0f-1.000000f,
    0.333333f, 1.0f-0.500000f,
    0.666667f, 1.0f-1.000000f,
    1.000000f, 1.0f-1.000000f,
    0.666667f, 1.0f-0.500000f
};
