#version 330 core

/* Define a variable vp_modelspace to hold layout of vtx attrib location 0 */
layout(location = 0) in vec3 vp_modelspace;

void
main() {
    gl_Position.xyz = vp_modelspace;
    gl_Position.w = 1.0;
}
