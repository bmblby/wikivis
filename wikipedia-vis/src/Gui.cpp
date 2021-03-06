#include "Gui.h"

namespace vta
{

using namespace nanogui;

// Gui::Gui(GLFWwindow* window, Controller* ctrl):
Gui::Gui(GLFWwindow* window, Model& model, View& view):
    _glfwWindow(window),
    _threshold(model._threshold),
    _view(view),
    _model(model)
    // _ctrl(ctrl)
{
    _screen = new nanogui::Screen();
    _screen->initialize(_glfwWindow, true);

    glfwGetWindowSize(_glfwWindow, &_width, &_height);
    _gui = new nanogui::FormHelper(_screen);
}

bool
Gui::contains(double x, double y)
{
    Eigen::Vector2i pos((int)x, (int)y);
    if(_searchBox->contains(pos) or
        _sliderBox->contains(pos))
    {
        return true;
    }
    else {
        return false;
    }
}

void
Gui::search_box(glm::vec3 pos, int width, int height)
{
    _searchBox = _gui->addWindow(Eigen::Vector2i(0, 0), "");
    _searchBox->setPosition(Eigen::Vector2i(_width - 370, 10));
    _searchBox->setLayout(new BoxLayout(
        Orientation::Horizontal,
        Alignment::Middle, 4, 4));

    TextBox* textbox = new TextBox(_searchBox);
    textbox->setFixedSize(Eigen::Vector2i(250, 20));
    textbox->setFontSize(16);
    textbox->setAlignment(TextBox::Alignment::Left);
    textbox->setEditable(true);
    textbox->setValue("Computer science");

    auto intBox = new IntBox<int>(_searchBox);
    intBox->setEditable(true);
    intBox->setFixedSize(Eigen::Vector2i(50, 20));
    intBox->setValue(_model._max_depth);
    // intBox->setUnits("depth");
    intBox->setDefaultValue("0");
    intBox->setFontSize(16);
    intBox->setFormat("[1-9][0-9]*");
    intBox->setSpinnable(true);
    intBox->setMinMaxValues(1, 10);
    intBox->setValueIncrement(1);

    Button* b = new Button(_searchBox, "Go!");
    auto default_col = b->backgroundColor();
    b->setCallback([=] {
        std::string name = textbox->value();
        size_t depth = intBox->value();
        Category cat;
        std::string root = _model._graph[_model._root].title;
        if(name == root) {
            _model.find(name, cat);
            std::cout << "don't build new graph\n";
            if(depth > _model._max_depth) {
                _model.expand(depth);
                _model.relayout(_width, _height);
                _model._dirty = true;
                _view.label_leaves();
            }
            else if(depth < _model._max_depth) {
                std::cout << "hide categories!!\n";
            }

        }
        else if(_model.find(name, cat)) {
            std::cout << "Found cat: " << cat.title << "\n";
            _model.initIDDFS(cat, depth);
            _model.layout(cat, _width, _height, depth, _model._r);
            _model._dirty = true;
            _view.label_free_tree();
            b->setBackgroundColor(default_col);
            return true;
        } else {
            textbox->setValue("please try another category");
            b->setBackgroundColor(Color(230,0,0,255));
            std::cout << "Input not found please try again\n";
            return false;
        }
    });
    _screen->performLayout();
}

void
Gui::slider_threshold(glm::vec3 pos, int width)
{
    _sliderBox = _gui->addWindow(Eigen::Vector2i(0, 0), "");
    _sliderBox->setPosition(Eigen::Vector2i(_width - 270, pos[1]));
    _sliderBox->setLayout(new BoxLayout(
        Orientation::Horizontal,
        Alignment::Middle, 4, 4));

    Slider* s = new Slider(_sliderBox);

    auto textBox = new FloatBox<float>(_sliderBox);
    textBox->setFixedSize(Eigen::Vector2i(50, 20));
    textBox->setValue(_model._threshold);
    textBox->setFontSize(16);

    s->setValue(_model._threshold);
    s->setFixedWidth(width);
    s->setCallback([=](float value) {
        _threshold = value;
        textBox->setValue(value);
    });
    s->setFinalCallback([=](float value) {
        _threshold = value;
        textBox->setValue(value);
        _model.threshold(value);
        _model._dirty = true;
    });
    _screen->performLayout();
}

void
Gui::display()
{
   _screen->drawContents();
   _screen->drawWidgets();
    // _screen->drawAll();
    _screen->setVisible(true);
}

void
Gui::redraw()
{
    // destroy all elements
    _screen->disposeWindow(_searchBox);
    _screen->disposeWindow(_sliderBox);
    search_box(glm::vec3(_width-370, 10, 0), 45, 25);
    slider_threshold(glm::vec3(_width-220, 50, 0), 200);
}

void
Gui::resizefun(int width, int height)
{
    _screen->resizeCallbackEvent(width, height);
    _width = width;
    _height = height;
    redraw();
}

void
Gui::mousebuttonfun(int button, int action, int mods)
{
    _screen->mouseButtonCallbackEvent(button, action, mods);
}

void
Gui::scrollfun(double x, double y)
{
    _screen->scrollCallbackEvent(x, y);
}

void
Gui::cursorfun(double xpos, double ypos)
{
    _screen->cursorPosCallbackEvent(xpos, ypos);
}

void
Gui::keyfun(int key, int scancode, int action, int mods)
{
    _screen->keyCallbackEvent(key, scancode, action, mods);
}
void
Gui::charfun(unsigned int codepoint)
{
    _screen->charCallbackEvent(codepoint);
}

} // namespace vta
