#include "View.h"

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

namespace vta
{

// View::View(Model& model, Gui& gui, GLFWwindow* window):
View::View(Model& model, GLFWwindow* window, glm::mat4& m, glm::mat4& view, glm::mat4& proj):
_model(model),
_modelM(m),
_viewM(view),
_projM(proj),
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
View::label_machine()
{
    for(auto label : labels) {
        auto pair = label.second;
        auto pos = pair.first;
        glm::vec3 view_pos = project(pos[0], pos[1]);
        auto posM = _modelM * glm::vec4(pos[0], pos[1], 0, 0);
        float angle = atan2(posM[1], posM[0]);
        set_label(view_pos, pair.second, angle);
    }
}

void
View::set_label(glm::vec3 const& pos, std::string const& title, float angle)
{
        nvgSave(_vg);
        nvgTranslate(_vg, pos[0], pos[1]);
        // float angle = atan2(pos_model[1], pos_model[0]);
        nvgRotate(_vg, angle);
        nvgFontSize(_vg, 16.0f);
        nvgFontFace(_vg, "verdana");
        nvgFillColor(_vg, nvgRGBA(243,245,248,255));
        nvgTextAlign(_vg, NVG_ALIGN_LEFT);
        nvgText(_vg, 0, 0, title.c_str(), NULL);
        nvgRestore(_vg);
        nvgRestore(_vg);
}

void
View::label_free_tree()
{
    auto vp = boost::vertices(_model._graph);
    for(; vp.first != vp.second; vp.first++) {
        auto title = _model._graph[*vp.first].title;
        auto pos = _model._graph[*vp.first].pos;
        auto pos_model = _modelM * glm::vec4(pos[0], pos[1], 0, 0);
        //debug
        // std::cout << "title: " << title;
        // std::cout << "local pos: " << pos[0] << " : " << pos[1] << std::endl;
        // std::cout << "model pos: " << pos_model[0] << " : " << pos_model[1] << std::endl;
        glm::vec3 view_pos = project(pos[0], pos[1]);
        size_t level = _model._graph[*vp.first].level;
        if(level == _model._max_depth or level == 1) {
            float angle = atan2(pos_model[1], pos_model[0]);
            set_label(view_pos, title, angle);

            //debug
            // break;
        }
    }
}

void
View::label_children(Category cat)
{
    auto pair = _model.in_graph(_model._graph, cat);
    auto parent = pair.second;
    auto ep = out_edges(parent, _model._graph);
    for(; ep.first != ep.second; ++ep.first) {
        auto child = target(*ep.first, _model._graph);
        auto pos = _model._graph[child].pos;
        auto title = _model._graph[child].title;
        auto pos_model = _modelM * glm::vec4(pos[0], pos[1], 0, 0);
        glm::vec3 view_pos = project(pos[0], pos[1]);
        float angle = atan2(pos_model[1], pos_model[0]);
        set_labe(view_pos, title, angle);
    }
}

void
View::HUD()
{
    //string to show
    std::string cat = "categories: " + std::to_string(_model._categories.size());;
    std::string art = "articles: " + std::to_string(_model._simM.size());

    nvgFontSize(_vg, 20.0f);
    nvgFontFace(_vg, "verdana");
    nvgFillColor(_vg, nvgRGBA(243,245,248,255));
    nvgTextAlign(_vg, NVG_ALIGN_LEFT);

    nvgTranslate(_vg, 5, 20);
    nvgText(_vg, 0, 0, _hover_cat.c_str(), NULL);
    nvgTranslate(_vg, 0, 20);
    nvgText(_vg, 0, 0, cat.c_str(), NULL);
    nvgTranslate(_vg, 0, 20);
    nvgText(_vg, 0, 0, art.c_str(), NULL);
}

glm::vec3
View::project(double x, double y)
{
    glm::vec3 m_pos = glm::vec3(x, y, 0);
    // std::cout << "model pos (" << glm::to_string(m_pos) << ")\n";
    // invert model matrix to fit nanovg projection
    glm::mat4 modelM = glm::scale(_modelM, glm::vec3(1, -1, 1));
    glm::vec3 s_pos = glm::project(m_pos, modelM * _viewM, _projM, glm::vec4(0, 0, _width, _height));
    // std::cout << "glm proj: (" << glm::to_string(s_pos) << ")\n";
    return s_pos;
}

} // namspace vta