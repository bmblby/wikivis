#include "View.h"

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

namespace vta
{

// View::View(Model& model, Gui& gui, GLFWwindow* window):
View::View(Model& model, GLFWwindow* window, glm::mat4& m, glm::mat4& view, glm::mat4& proj):
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
View::set_labels()
{
    auto vp = boost::vertices(_model._graph);
    for(; vp.first != vp.second; vp.first++) {
        auto title = _model._graph[*vp.first].title;
        auto pos = _model._graph[*vp.first].pos;
        // std::cout << "model pos: " << pos[0] << " : " << pos[1] << std::endl;
        glm::vec3 view_pos = project(pos[0], pos[1]);

        if(_model._graph[*vp.first].level == 1) {
            nvgFontSize(_vg, 20.0f);
            nvgFontFace(_vg, "verdana");
            nvgFillColor(_vg, nvgRGBA(243,245,248,255));
            // nvgFillColor(_vg, nvgRGBA(46,59,69,255));
            nvgTextAlign(_vg, NVG_ALIGN_LEFT);
            nvgText(_vg, view_pos[0]+4, view_pos[1]+4, title.c_str(), NULL);
        }
        if(_model._graph[*vp.first].level == _model._max_depth) {
            float angle = atan2(pos[1], pos[0]);
            std::cout << "angle: " << angle << std::endl;
            std::cout << "pos: " << pos[0] << " : " << pos[1] << std::endl;

            nvgSave(_vg);
            nvgTranslate(_vg, view_pos[0], view_pos[1]);

            // float radius =_model._r + (_model._r / _model._max_depth) * 2;
            // float rad = acos(pos[0]/radius);
            // if(pos[1] < 0) {
            //     rad = 2*M_PI -rad;
            // }
            // nvgRotate(_vg, rad);

            // nvgSave(_vg);
            nvgRotate(_vg, angle);

            nvgSave(_vg);
            nvgFontSize(_vg, 12.0f);
            nvgFontFace(_vg, "verdana");
            nvgFillColor(_vg, nvgRGBA(243,245,248,255));
            nvgTextAlign(_vg, NVG_ALIGN_LEFT);
            nvgText(_vg, 0, 0, title.c_str(), NULL);
            nvgRestore(_vg);
            nvgRestore(_vg);
        }
    }

}

glm::vec3
View::project(double x, double y)
{
    glm::vec3 m_pos = glm::vec3(x, y, 0);
    // std::cout << "model pos (" << glm::to_string(m_pos) << ")\n";
    // invert model matrix to fit nanovg projection
    glm::mat4 model_mat = glm::scale(_model_mat, glm::vec3(1, -1, 1));
    glm::vec3 s_pos = glm::project(m_pos, model_mat * _view_mat, _proj_mat, glm::vec4(0, 0, _width, _height));
    // std::cout << "glm proj: (" << glm::to_string(s_pos) << ")\n";
    return s_pos;
}

} // namspace vta