#ifndef VTA_CONTROLLER
#define VTA_CONTROLLER

//gloost
#include <gloost/human_input/MouseState.h>

// vta
#include "Model.h"
#include "Renderer.h"
#include "View.h"
#include "Gui.h"

namespace vta
{

class Controller
{
  public:
    // class constructor
    Controller(Model&, Renderer&, View&, Gui& gui);

    // mouse input
    void mousePress(int x, int y, int btn, int mods);
    void mouseRelease(int x, int y, int btn, int mods);
    void mouseMove(int x, int y, int state);
    void mouseScroll(float);
    void reset_mouse();

    // key input
    void keyPress(int key, int mods);
    void keyRelease(int key, int mods);

    //util
    bool find(std::string const& name, int depth) const;
    void hover(int x, int y) const;


  protected:
    Model& _model;
    Renderer& _renderer;
    View& _view;
    Gui& _gui;

    // holding the current mouse state
    glm::vec3 _start;
    glm::vec3 _last = glm::vec3(0,0,0);
    gloost::human_input::MouseState _mouse;
    gloost::Vector3 _key_state;

    // Toggle if strg-key is pressed
    bool _strg_key_pressed;
};

} // namespace vta
#endif // VTA_CONTROLLER
