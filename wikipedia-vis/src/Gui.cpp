#include "Gui.h"

namespace vta
{

using namespace nanogui;

Gui::Gui(GLFWwindow* window, Ctrl* ctrl):
    _glfwWindow(window),
    _ctrl(ctrl)
{
    _screen = new nanogui::Screen();
    _screen->initialize(_glfwWindow, true);

    _gui = new nanogui::FormHelper(_screen);
}

bool
Gui::contains(double x, double y)
{
    // TODO write function returning mouse iover widget windows
    Eigen::Vector2i pos((int)x, (int)y);
    if(_window->contains(pos)) {
        // _window->
        std::cout << "Position: " << pos << " is inside gui!";
        return true;
    }
    else {
        std::cout << "Position: " << pos << " is NOT inside gui!";
        return false;
    }
}

void
Gui::search_box(glm::vec3 pos, int width, int height)
{
    _window = _gui->addWindow(Eigen::Vector2i(0, 0),
        "Search Category");
    _window->setPosition(Eigen::Vector2i(pos[0], pos[1]));
    _window->setLayout(new BoxLayout(
        Orientation::Horizontal,
        Alignment::Middle, 4, 4));

    TextBox* textbox = new TextBox(_window);
    textbox->setFixedSize(Eigen::Vector2i(120, 20));
    textbox->setFontSize(15);
    textbox->setAlignment(TextBox::Alignment::Left);
    textbox->setEditable(true);
    textbox->setValue("Computer Science");
    // textbox->keyboardEvent()
    std::cout << textbox->value();

    //TODO write callback to query wikidb for inserted category
    Button* b = new Button(_window, "Go!");
    b->setCallback([=] {
        std::cout << textbox->value();
        // _ctrl-
    } );

    // std::string strvar = "Computer Science";
    // bool enalbe = true;
    // _gui->addVariable("search: ", strvar);

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
