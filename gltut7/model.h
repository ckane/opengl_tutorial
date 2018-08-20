#ifndef _CK_MODEL_H
#define _CK_MODEL_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// OpenGL Math headers (for GLSL)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
 * Designed to load a simple Wavefront (*.obj)
 * text-based model. Give the filename of the
 * model in the constructor.
 */
class ObjModel {
    public:
        ObjModel(const char *filename);
        void init(void);
        void colorize(void);
        void place(void);
        void render(const glm::mat4&);

        const std::vector<glm::vec3> &getVertexes(void) const {return ovtx; };
        const std::vector<glm::vec2> &getUV(void) const {return ouvs; };
        const std::vector<glm::vec3> &getNormals(void) const {return onorm; };

    private:
        const char *obj_file_name;
        bool valid;
        void load_object(void);

        /* The vector sets. */
        std::vector<glm::vec3> ovtx, onorm;
        std::vector<glm::vec2> ouvs;

        /* GL params. */
        GLuint varr, vbuf, uvbuf, progid, mvp;
};

#endif /* _CK_MODEL_H */
