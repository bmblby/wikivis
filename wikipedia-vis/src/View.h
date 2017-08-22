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
    void label_machine();
    void set_label(glm::vec3 const& pos, std::string const& title, float angle);
    void label_free_tree();
    void label_children(Category parent);
    void HUD();

    glm::vec3 project(double x, double y);

    //member
    std::map<uint32_t, std::tuple<glm::vec3, std::string, float>> labels;

    Model& _model;
    GLFWwindow* _window;
    NVGcontext* _vg;
    glm::mat4& _modelM;
    glm::mat4& _viewM;
    glm::mat4& _projM;

    // Gui& _gui;
    int _width;
    int _height;
    float _pxRatio;
    int _font;
    size_t _max_depth;
    std::string _hover_cat;
};

} // namespace vta
#endif // VTA_VIEW
