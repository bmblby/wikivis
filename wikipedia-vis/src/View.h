#ifndef VTA_VIEW
#define VTA_VIEW

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <nanovg.h>

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
    View(Model& model,  GLFWwindow* window);
    void cleanup();

    //draw
    void drawHomeView();
    void drawBubble () const;

    void panning(glm::vec3 const& vec);

    //member
    Model& _model;
    GLFWwindow* _window;
    NVGcontext* _vg;
    // Gui& _gui;
    int _width;
    int _height;
    float _pxRatio;

};

} // namespace vta
#endif // VTA_VIEW
