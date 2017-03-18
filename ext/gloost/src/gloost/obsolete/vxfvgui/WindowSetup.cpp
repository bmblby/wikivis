
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



// gloost system includes
#include <gloost/vxfvgui/VxfvGui.h>
#include <gloost/vxfvgui/WindowSetup.h>
#include <gloost/vxfvgui/WindowWidget.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   WindowSetup

  \brief   ...

  \author  Felix Weiszig
  \date    May 2011
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

WindowSetup::WindowSetup(VxfvGui* gui,
                         unsigned screenWidth,
                         unsigned screenHeight):
    _gui(gui),
    _windowWidget(0),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

WindowSetup::~WindowSetup()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the WindowWidget
  \param   ...
  \remarks ...
*/

WindowWidget*
WindowSetup::getWindowWidget()
{
  return _windowWidget;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   frame step
  \param   ...
  \remarks ...
*/

/*virtual*/
void
WindowSetup::frameStep()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   resize screen
  \param   ...
  \remarks ...
*/

/*virtual*/
void
WindowSetup::resize(unsigned screenWidth, unsigned screenHeight)
{
  _screenWidth  = screenWidth;
  _screenHeight = screenHeight;
}


////////////////////////////////////////////////////////////////////////////////


} // namespace gloost


