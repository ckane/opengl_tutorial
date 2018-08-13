#version 330 core
// Input vertex data
layout(location = 0) in vec3 vp_modelspace;
layout(location = 1) in vec3 vp_color;

// Values that stay constant for whole mesh
uniform mat4 MVP;

// Outputs
out vec3 frag_color;

void main() {
    // Output position of vertex, in clip space
    gl_Position = MVP * vec4(vp_modelspace, 1);
    frag_color = vp_color;
}
