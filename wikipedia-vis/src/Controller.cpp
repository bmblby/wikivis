#include "Controller.h"

namespace vta
{

Controller::Controller(Model& model, Renderer& renderer):
_model(model),
_renderer(renderer),
_mouse_state(),
_key_state(),
_strg_key_pressed(false)
{}

void
Controller::mousePress(int x, int y, int btn, int mods)
{
    _mouse_state.setButtonState(btn, true);
    std::cout << "button number: " << btn <<std::endl;
}

void
Controller::mouseRelease(int x, int y, int btn, int mods)
{
    if(_mouse_state.getButtonState(btn)){
        _mouse_state.setButtonState(btn, false);
        std::cout << "Set  mouse button to false\n";
    }

}

void
Controller::mouseMove(int x, int y, int state)
{
    _mouse_state.setPosition((float) x, (float) y);

    if(_mouse_state.getButtonState(GLOOST_MOUSESTATE_BUTTON0) ) {
        gloost::Point3 lastPos = _mouse_state.getLastMouseDownPosition();
        glm::vec3 vec(x - lastPos[0], y - lastPos[1], 0.0f);
        glm::vec3 inv_vec(-vec.x, -vec.y, -vec.z);
        _renderer.translate(inv_vec);
    }
    _renderer.hover(glm::vec3(x, y, 0.0));
}

void
Controller::mouseScroll(float yoffset)
{
    // Zoom in
    _renderer.zoom(yoffset);
    _renderer.set_mouse(_mouse_state);
}

void
Controller::reset_mouse_state()
{
  // reset mouse events
  _mouse_state.resetMouseEvents();
  _mouse_state.setSpeedToZero();
}

void
Controller::keyPress(int key, int mods)
{
  // http://www.glfw.org/docs/latest/group__keys.html
    switch (key)
    {
      case 82: //GLFW_KEY_R
      {
        _renderer.redraw();
        break;
      }

      case 257: // ENTER
      {
        // open wikipage of selected node
  //      if (_highlight_nodes_mode)
  //      {
  //        // Open firefox window with corresponding wikipedia article
  //        std::string url = "http://en.wikipedia.org/w/index.php?oldid=" + boost::lexical_cast<std::string>(_highlighted_node->_article.revid);
  //        std::string command = "firefox " + url;
  //
  //        system(command.c_str());
  //      }
        break;
      }

      case 341: // strg
      {
        _strg_key_pressed = true;

        break;
      }

      case 256: // ESC
      {
        break;
      }

      default:
      {}
    }
}

void
Controller::keyRelease(int key, int mods)
{
  switch (key)
  {
    case 341: // strg
    {
      _strg_key_pressed = false;
      break;
    }
  }
}

void
Controller::find(std::string const& name, int depth)
{
    Category cat;
    if(_model.find(name, cat)) {
        std::cout << "Found cat: " << cat.title << "\n";
        // Graph g = _model.graph(cat);
        _model._graph = _model.graph(cat, depth);
        _model._dirty = true;
        auto fr_map = _model.layout_FR();
        _model.write_layout(fr_map);
    } else {std::cout << "Input not found please try again\n";}
}

} // namespace vta
