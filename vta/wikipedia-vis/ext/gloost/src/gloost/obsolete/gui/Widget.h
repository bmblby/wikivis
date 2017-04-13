
/*
                       ___                            __
                      /\_ \                          /\ \__
                   __ \//\ \     ___     ___     ____\ \  _\
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/
                   \_/__/

                   OpenGL framework for fast demo programming

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).

            gloost is being created by Felix Weiﬂig and Stephan Beck

     Felix Weiﬂig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/



#ifndef GLOOST_WIDGET_H
#define GLOOST_WIDGET_H


// gloost includes
#include <gloost/gl/UniformSet.h>
#include <gloost/Vector3.h>
#include <gloost/human_input/MouseState.h>


/// ggui includes
#include <gloost/PropertySet.h>


// cpp includes
#include <string>
#include <list>


namespace gloost
{



class Gui;


  ///  Base class of all Gui elements

class Widget: public PropertySet
{
	public:

    Widget(std::string type, std::string name = "unnamed widget");


    /// destructor
    virtual ~Widget();



    /// loads all necessary resources for this node
    virtual void loadResources();

    /// check the a point if it's inside the rect of this node
    bool isMouseInside(const gloost::Point3& loc);

    /// main event creating method, returns 1 if the mouse was inside this widget
    bool handleMouse(gloost::human_input::MouseState* mouse);


    /// draw this node and all child
    void draw();

    /// render this particular node
    virtual void onDraw();



    /// handle a incomming char (textinput)
    bool handleKey(const unsigned char& asciiChar);
    virtual bool onHandleKey(const unsigned char& asciiChar);


    /// get the unique id of this node
//    unsigned int getId();


    // intern event automatic and extern callbacks
    void mouseDown();
    virtual void onMouseDown();

    void mouseUp();
    virtual void onMouseUp();

    void mouseUpOutside();
    virtual void onMouseUpOutside();

    void mouseEnter();
    virtual void onMouseEnter();

    void mouseLeave();
    virtual void onMouseLeave();

    void mouseWithin();
    virtual void onMouseWithin();

    void enterFrame();
    virtual void onEnterFrame();

    void exitFrame();
    virtual void onExitFrame();


    /// gui object uses this methode to add a pointer to this widget
    void setGui(Gui* gui);


	protected:

    /// a reference to the gui
    Gui* _gui;
};


} // namespace gloost


#endif // GLOOST_WIDGET_H



