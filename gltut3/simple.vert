#version 330 core
// Input vertex data
layout(location = 0) in vec3 vp_modelspace;

// Values that stay constant for whole mesh
uniform mat4 MVP;

void main() {
    // Output position of vertex, in clip space
    gl_Position = MVP * vec4(vp_modelspace, 1);
}
