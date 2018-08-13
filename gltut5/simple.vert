#version 330 core
// Input vertex data
layout(location = 0) in vec3 vp_modelspace;
layout(location = 1) in vec2 vp_uv;

// We output the uv values for the texture vertex position
out vec2 uv;

// Values that stay constant for whole mesh
uniform mat4 MVP;

// Outputs
out vec3 frag_color;

void main() {
    // Output position of vertex, in clip space
    gl_Position = MVP * vec4(vp_modelspace, 1);

    // We pass the uv coordinate out
    uv = vp_uv;
}
