#version 330 core

/* Interpolated values from vertex shaders. */
in vec2 uv;

/* Output data. */
out vec3 color;

/* Value stays constant for whole mesh. */
uniform sampler2D myTexSampler;

void main() {
    /* Output the color of the texture at the given u, v point. */
    color = texture(myTexSampler, uv).rgb;
}
