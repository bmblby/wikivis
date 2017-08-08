#include "Controller.h"

namespace vta
{

Controller::Controller(Model& model, Renderer& renderer, View& view, Gui& gui):
_model(model),
_renderer(renderer),
_view(view),
_gui(gui),
_mouse(),
_key_state(),
_strg_key_pressed(false)
{}

void
Controller::mousePress(int x, int y, int btn, int mods)
{
    _mouse.setButtonState(btn, true);
    //std::cout << "button number: " << btn <<std::endl;

    //check category was clicked
    Category cat;
    auto vec = _renderer.screen2modelSpace(glm::vec3(x, y, 0.0));
    if(_model.pos2cat(vec, cat) and btn == 0) {
        _model.expand(cat);
        // std::cout << cat;
    }
    _mouse.setButtonState(btn, true);
    if(_mouse.getButtonState(btn)){
        _start = glm::vec3(x - _last.x, y - _last.y, 0.0);
        // std::cout << "btn: " << btn << glm::to_string(_start) << std::endl;
    }
}

void
Controller::mouseRelease(int x, int y, int btn, int mods)
{
    if(_mouse.getButtonState(btn)){
        _mouse.setButtonState(btn, false);
        _last = glm::vec3(x - _start.x, y - _start.y, 0.0);
        // std::cout << "btn: " << btn << glm::to_string(_last) << std::endl;
    }

}

void
Controller::mouseMove(int x, int y, int state)
{
    _mouse.setPosition((float) x, (float) y);
    if(_gui.contains(x, y))
        _gui.cursorfun(x, y);

    if(_mouse.getButtonState(GLOOST_MOUSESTATE_BUTTON0) ) {
        glm::vec3 vec = glm::vec3(x - _start.x, y - _start.y, 0.0);
        _renderer.translate(vec);
    }
    if(hover_state == true)
        hover(x, y);
}

void
Controller::mouseScroll(float yoffset)
{
    // Zoom in
    if(zoom_state)
        //BUG no position transaltion
        _renderer.zoomFOV(yoffset);
    else
        _renderer.zoom(yoffset);
    _renderer.set_mouse(_mouse);
}

void
Controller::reset_mouse()
{
  // reset mouse events
  _mouse.resetMouseEvents();
  _mouse.setSpeedToZero();
}

void
Controller::keyPress(int key, int mods)
{
    // http://www.glfw.org/docs/latest/group__keys.html
    switch (key)
    {
        case 321: //GLFW_KEY_KP_1
        {
            hover_state = !hover_state;
            if(hover_state)
                std::cout << "activate hover!\n";
            else
                std::cout << "deactivate hover!\n";
            break;
        }
    case 322: //GLFW_KEY_KP_2
        {
            //default state FOV
            zoom_state = !zoom_state;
            if(zoom_state)
                std::cout << "zoomFOV activ!\n";
            else
                std::cout << "zoom(scale) activ!\n";
        }

    case 323: //GLFW_KEY_KP_3
        {
            //default state perspective
            proj_state = !proj_state;
            if(proj_state) {
                _renderer._projectionMatrix = _renderer._perspMat;
                std::cout << "Perspective Projection\n";
            }
            else {
                _renderer._projectionMatrix = _renderer._orthoMat;
                std::cout << "Orthogonal Projection\n";
            }
        }
        // case 257: // ENTER
        // {
        //     // open wikipage of selected node
        //     if (_highlight_nodes_mode)
        //     {
        //         // Open firefox window with corresponding wikipedia article
        //         std::string url = "http://en.wikipedia.org/w/index.php?oldid=" + boost::lexical_cast<std::string>(_highlighted_node->_article.revid);
        //         std::string command = "firefox " + url;
        //
        //         system(command.c_str());
        //     }
        //     break;
        // }

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

bool

Controller::find(std::string const& name, int depth) const
{
    Category cat;
    if(_model.find(name, cat)) {
        std::cout << "Found cat: " << cat.title << "\n";
        _model.initGraph(cat, depth);
        _model._dirty = true;
        auto map = _model.layout_FR();
        _model.write_layout(map);
        return true;
    } else {
        std::cout << "Input not found please try again\n";
        return false;
    }
}

void
Controller::hover(int x, int y) const
{
    auto vec = _renderer.screen2modelSpace(glm::vec3(x, y, 0.0));
    Category cat;
    if(_model.pos2cat(vec, cat))
        std::cout << cat << std::endl;
}

} // namespace vta
