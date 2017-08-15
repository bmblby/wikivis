#include "Gui.h"

namespace vta
{

using namespace nanogui;

// Gui::Gui(GLFWwindow* window, Controller* ctrl):
Gui::Gui(GLFWwindow* window, Model& model):
    _glfwWindow(window),
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
    if(_window->contains(pos)) {
        return true;
    }
    else {
        return false;
    }
}

void
Gui::search_box(glm::vec3 pos, int width, int height)
{
    _window = _gui->addWindow(Eigen::Vector2i(0, 0), "");
    _window->setPosition(Eigen::Vector2i(pos[0], pos[1]));
    _window->setLayout(new BoxLayout(
        Orientation::Horizontal,
        Alignment::Middle, 4, 4));

    TextBox* textbox = new TextBox(_window);
    textbox->setFixedSize(Eigen::Vector2i(250, 20));
    textbox->setFontSize(16);
    textbox->setAlignment(TextBox::Alignment::Left);
    textbox->setEditable(true);
    textbox->setValue("Computer science");

    auto intBox = new IntBox<int>(_window);
    intBox->setEditable(true);
    intBox->setFixedSize(Eigen::Vector2i(50, 20));
    intBox->setValue(2);
    // intBox->setUnits("depth");
    intBox->setDefaultValue("0");
    intBox->setFontSize(16);
    intBox->setFormat("[1-9][0-9]*");
    intBox->setSpinnable(true);
    intBox->setMinMaxValues(0, 5);
    intBox->setValueIncrement(1);

    Button* b = new Button(_window, "Go!");
    auto default_col = b->backgroundColor();
    b->setCallback([=] {
        std::string name = textbox->value();
        size_t depth = intBox->value();
        Category cat;
        std::string root = _model._graph[_model._root].title;
        if(name == root)
            std::cout << "don't build new graph\n";
        if(_model.find(name, cat)) {
            std::cout << "Found cat: " << cat.title << "\n";
            _model.initIDDFS(cat, depth);
            _model.layout(cat, _width, _height, depth, 0.7f);   //0.7 radius
            _model._dirty = true;
            b->setBackgroundColor(default_col);
            return true;
        } else {
            textbox->setValue("please try another category");
            b->setBackgroundColor(Color(230,0,0,255));
            std::cout << "Input not found please try again\n";
            return false;
        }
    } );

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
Gui::resizefun(int width, int height)
{
    _screen->resizeCallbackEvent(width, height);
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
