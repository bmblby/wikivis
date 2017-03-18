
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



#ifndef H_VXFV_HORIZONTALTITLEWIDGET
#define H_VXFV_HORIZONTALLINE



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


  //  A horizontal line with a title to use with the VxfvGui

class HorizontalTitleWidget: public VxfvWidget
{
	public:

    // class constructor
    HorizontalTitleWidget(VxfvGui* gui);

    // class destructor
	  virtual ~HorizontalTitleWidget();


    virtual void onInitWidget();
    virtual void onUpdateMesh();

    virtual void onEnterFrame();
    virtual void onExitFrame();


    virtual void onDraw();


	private:





};


} // namespace gloost


#endif // H_VXFV_HORIZONTALTITLEWIDGET


