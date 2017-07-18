#ifndef VTA_CONTROLLER
#define VTA_CONTROLLER

//gloost
#include <gloost/human_input/MouseState.h>

// vta
#include "Model.h"
#include "Renderer.h"

namespace vta
{

class Ctrl
{
  public:
    // class constructor
    Ctrl(Model&, Renderer&);

    // mouse input
    void mousePress(int x, int y, int btn, int mods);
    void mouseRelease(int x, int y, int btn, int mods);
    void mouseMove(int x, int y, int state);
    void mouseScroll(float);
    void reset_mouse_state();

    // key input
    void keyPress(int key, int mods);
    void keyRelease(int key, int mods);

    //util
    bool find(std::string const& name, int depth) const;
    void hover(int x, int y) const;


  protected:
    Model& _model;
    Renderer& _renderer;

    // holding the current mouse state
    gloost::human_input::MouseState _mouse_state;
    gloost::Vector3 _key_state;

    // Toggle if strg-key is pressed
    bool _strg_key_pressed;
};

} // namespace vta
#endif // VTA_CONTROLLER
