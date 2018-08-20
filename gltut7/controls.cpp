// OpenGL headers
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.h"

using namespace glm;

Controls::Controls(GLFWwindow *mainwin) : position(0, 0, 5), h_angle(3.14f), v_angle(0.f),
                           init_fov(45.f), speed(3.f), mspeed(0.05f), win(mainwin),
                           last_time(0.f) {
};

void
Controls::updateControls(void) {
    int win_w, win_h;
    double mx, my;
    float deltaTime;

    if(last_time == 0.f) {
        deltaTime = 0.f;
    } else {
        deltaTime = glfwGetTime() - last_time;
    };

    /* Read mouse cursor movement, and window size, since last call. */
    glfwGetCursorPos(this->win, &mx, &my);
    glfwGetWindowSize(this->win, &win_w, &win_h);

    /* Update the horizontal/vertical angles. */
    this->h_angle += mspeed * deltaTime * (float(win_w)/2.f - float(mx));
    this->v_angle += mspeed * deltaTime * (float(win_h)/2.f - float(my));

    /* At the end of updating, anchor the cursor back to the middle of the window. */
    glfwSetCursorPos(this->win, double(win_w)/2., double(win_h)/2.);

    /* Update time. */
    last_time = glfwGetTime();
};

mat4
Controls::getProjection(void) {
    float fov = this->init_fov;
    return perspective(radians(fov), 4.f/3.f, 0.1f, 100.f);
};

mat4
Controls::getView(void) {
    vec3 direction(cos(v_angle) * sin(h_angle),
                   sin(v_angle),
                   cos(v_angle) * cos(h_angle));
    vec3 right(sin(h_angle - 3.14f/2.f),
               0.f,
               cos(h_angle - 3.14f/2.f));
    vec3 up = cross(right, direction);
    float deltaTime;

    if(last_time == 0.f) {
        deltaTime = 0.f;
    } else {
        deltaTime = glfwGetTime() - last_time;
    };

    if(glfwGetKey(this->win, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    };

    if(glfwGetKey(this->win, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    };

    if(glfwGetKey(this->win, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    };

    if(glfwGetKey(this->win, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    };

    return lookAt(position, position+direction, up);
};
