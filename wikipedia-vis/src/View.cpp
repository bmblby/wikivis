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
View::cleanup()
{
    nvgDeleteGL3(_vg);
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


    int center_x = _width/2;
    int center_y = _height/2;
    int center_r = 100;
    nvgBeginFrame(_vg, _width, _height, pxRatio);

    nvgBeginPath(_vg);
    nvgCircle(_vg, center_x, center_y, center_r);
    nvgStrokeColor(_vg, nvgRGBA(255,0,0,255));
    nvgStrokeWidth(_vg, 1.0f);
    nvgStroke(_vg);

    // fill circle vector with vertice pos and col
    float radius = 40.0f;
    for(float angle=0.0f; angle <= 360.0f; angle+=radius) {
        float rad = 3.14*angle/180;
        // float rad = 2*angle/20;
        float radius_c = center_r + radius;
        float xPos = center_x + radius_c * sin(rad);
        float yPos = center_y + radius_c * cos(rad);
        nvgBeginPath(_vg);
        nvgCircle(_vg, xPos, yPos, radius);
        nvgStrokeColor(_vg, nvgRGBA(255,255,255,255));
        nvgStrokeWidth(_vg, 1.0f);
        nvgStroke(_vg);

        auto font = nvgCreateFont(_vg, "verdana", "../../res/fonts/Verdana.ttf");
        if(font == -1)
            std::cout << "could not find font!\n";
        nvgFontSize(_vg, 10.0f);
        nvgFontFace(_vg, "verdana");
        nvgFillColor(_vg, nvgRGBA(255,255,255,255));
        nvgTextAlign(_vg, NVG_ALIGN_LEFT);
        nvgText(_vg, xPos-10.0f, yPos, "Testing Text", NULL);
    }

    nvgEndFrame(_vg);
}


void
View::drawHomeView()
{
  // _gui.search_box(glm::vec3(10, 10, 0), 45, 25);
  // _gui.display();

}

} // namspace vta