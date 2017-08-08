#ifndef VTA_VIEW
#define VTA_VIEW

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <nanovg.h>
#include <glm/glm.hpp>

// vta
#include "Model.h"
// #include "Gui.h"
// #include "Geometry.h"

namespace vta
{

struct View
{
    // class constructor
    // View(Model& model,  Gui& gui, GLFWwindow* window);
    View(Model& model,  GLFWwindow* window, glm::mat4& m, glm::mat4& view, glm::mat4& proj);
    void beginFrame();
    void endFrame();
    void cleanup();
    void resize();

    //draw
    void set_labels();

    glm::vec3 project(double x, double y);

    //member
    Model& _model;
    GLFWwindow* _window;
    NVGcontext* _vg;
    glm::mat4& _model_mat;
    glm::mat4& _view_mat;
    glm::mat4& _proj_mat;

    // Gui& _gui;
    int _width;
    int _height;
    float _pxRatio;
    int _font;

    size_t _max_depth;
};

} // namespace vta
#endif // VTA_VIEW
