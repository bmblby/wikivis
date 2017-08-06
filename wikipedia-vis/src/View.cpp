#include "View.h"

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

namespace vta
{

// View::View(Model& model, Gui& gui, GLFWwindow* window):
View::View(Model& model, GLFWwindow* window, glm::mat4 m, glm::mat4 view, glm::mat4 proj):
_model(model),
_model_mat(m),
_view_mat(view),
_proj_mat(proj),
_window(window)
// _gui(gui)
{
    glfwGetWindowSize(_window, &_width, &_height);
    _vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if(_vg == NULL) {
        std::cerr << "Could not init nanovg!" << std::endl;
        exit(EXIT_FAILURE);
    }
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(_window, &fbWidth, &fbHeight);
    _pxRatio = (float)fbWidth / (float) _width;

    _font = nvgCreateFont(_vg, "verdana", "../../res/fonts/Verdana.ttf");
    if(_font == -1)
        std::cout << "could not find font!\n";
}

void
View::beginFrame()
{
    nvgBeginFrame(_vg, _width, _height, _pxRatio);
}

void
View::endFrame()
{
    nvgEndFrame(_vg);
}

void
View::cleanup()
{
    nvgDeleteGL3(_vg);
}

void
View::resize()
{
    glfwGetWindowSize(_window, &_width, &_height);
}

void
View::drawBubble() const
{
    int center_x = _width/2;
    int center_y = _height/2;
    int center_r = 100;

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

        nvgFontSize(_vg, 10.0f);
        nvgFontFace(_vg, "verdana");
        nvgFillColor(_vg, nvgRGBA(255,255,255,255));
        nvgTextAlign(_vg, NVG_ALIGN_LEFT);
        nvgText(_vg, xPos-10.0f, yPos, "Testing Text", NULL);
    }

}


glm::vec3
View::project(double x, double y)
{
    glm::vec3 m_pos = glm::vec3(x, y, 0);
    // std::cout << "model pos (" << glm::to_string(m_pos) << ")\n";
    glm::vec3 s_pos = glm::project(m_pos, _model_mat * _view_mat, _proj_mat, glm::vec4(0, 0, _width, _height));
    // std::cout << "glm proj: (" << glm::to_string(s_pos) << ")\n";
    return s_pos;
}

} // namspace vta