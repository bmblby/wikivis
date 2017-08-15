#ifndef VTA_GUI
#define VTA_GUI

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <nanogui/nanogui.h>

// #include "Controller.h"
#include "Model.h"

namespace vta
{

class Gui
{
  public:
    // class constructor
    // Gui(GLFWwindow* window, Controller* ctrl);
    Gui(GLFWwindow* window, Model& model);

    bool contains(double x, double y);
    void search_box(glm::vec3 pos, int width, int height);
    void slider_threshold(glm::vec3 pos, int width);
    void display();

    // Callbacks
    void resizefun(int width, int height);
    void mousebuttonfun(int button, int action, int mods);
    void scrollfun(double x, double y);
    void cursorfun(double xpos, double ypos);
    void keyfun(int key, int scancode, int action, int mods);
    void charfun(unsigned int codepoint);


  protected:
    // Controller* _ctrl;
    Model& _model;
    GLFWwindow* _glfwWindow;

    nanogui::Screen* _screen;
    nanogui::Widget* _widget;
    nanogui::Window* _searchBox;
    nanogui::Window* _sliderBox;
    nanogui::FormHelper* _gui;

    int _width;
    int _height;
};

} // namespace vta
#endif // VTA_GUI
