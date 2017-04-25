
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



#ifndef H_VXFV_TEXTUREDBUTTONWIDGET
#define H_VXFV_TEXTUREDBUTTONWIDGET



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



  //  A button widget

class TexturedButtonWidget: public VxfvWidget
{
	public:

    // class constructor
    TexturedButtonWidget(VxfvGui*     gui,
                         unsigned     textureId,
                         gloost::vec4 texcoords = gloost::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                         unsigned     numStates = 3);

    // class destructor
	  virtual ~TexturedButtonWidget();


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


    unsigned     _textureId;
    gloost::vec4 _texcoords;
    unsigned     _numStates;


    bool        _value;


	private:





};


} // namespace gloost


#endif // H_VXFV_TEXTUREDBUTTONWIDGET


