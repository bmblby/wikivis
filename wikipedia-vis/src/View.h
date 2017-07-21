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

    void drawBubble () const;


    //draw
    void drawHomeView();
    // void drawCategoryGraphView();
    // void drawCategoryListView();
    // void drawArticleListView();
    //
    // void drawExpandCategory();


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
