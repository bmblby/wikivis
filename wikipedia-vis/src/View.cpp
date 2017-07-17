#include "View.h"

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

namespace vta
{

View::View(Model& model, Gui& gui, GLFWwindow* window):
_model(model),
_window(window),
// _vg(vg),
_gui(gui)
{
    glfwGetWindowSize(_window, &_width, &_height);
    _vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
}

void
View::drawBubble() const
{

    if(_vg == NULL) {
        std::cerr << "Could not init nanovg!" << std::endl;
        exit(EXIT_FAILURE);
    }
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(_window, &fbWidth, &fbHeight);
    float pxRatio = (float)fbWidth / (float) _width;


    int center_x = 150;
    int center_y = 150;
    int center_r = 100;
    nvgBeginFrame(_vg, _width, _height, pxRatio);

    nvgBeginPath(_vg);
    nvgCircle(_vg, center_x, center_y, center_r);
    nvgFill(_vg);
    nvgStrokeColor(_vg, nvgRGBA(0,0,0,64));
    nvgStrokeWidth(_vg, 1.0f);
    nvgStroke(_vg);

    nvgEndFrame(_vg);
}


void
View::drawHomeView()
{
  // _gui.search_box(glm::vec3(10, 10, 0), 45, 25);
  // _gui.display();

}

} // namspace vta