
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
*/


#ifndef H_VXFV_WINDOWBORDERWIDGET
#define H_VXFV_WINDOWBORDERWIDGET



/// vxfv includes
#include <gloost/vxfvgui/VxfvWidget.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <list>


namespace gloost
{

  class MouseState;


  //  Base class for all Widgets used by the gui

class WindowBorderWidget: public VxfvWidget
{
	public:

    // class constructor
    WindowBorderWidget(VxfvGui* gui);

    // class destructor
	  virtual ~WindowBorderWidget();


    virtual void onInitWidget();
    virtual void onUpdateMesh();

    virtual void onEnterFrame();
    virtual void onExitFrame();


    // HANDLER FOR MOUSE STATES AND EVENTS
    virtual void onMouseEnterEvent();
    virtual void onMouseLeaveEvent();
    virtual void onMouseDownEvent();
    virtual void onMouseUpEvent();
    virtual void onMouseDownOutsideEvent();
    virtual void onMouseUpOutsideEvent();
    virtual void onMouseWithin();
    virtual void onMouseOutside();



    virtual void onDraw();



    gloost::vec4 _texcoords;

    bool _dragable;


	private:

   bool _windowMinimized;



};


} // namespace gloost


#endif // H_VXFV_WINDOWBORDERWIDGET


