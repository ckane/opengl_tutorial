#ifndef _CK_CONTROLS_H
#define _CK_CONTROLS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Controls {
    glm::vec3 position;
    float h_angle, v_angle, init_fov, speed, mspeed;
    GLFWwindow *win;
    double last_time;

    public:
        Controls(GLFWwindow* mainwin);
        void updateControls(void);
        glm::mat4 getProjection(void);
        glm::mat4 getView(void);
};

#endif /* _CK_CONTROLS_H */
