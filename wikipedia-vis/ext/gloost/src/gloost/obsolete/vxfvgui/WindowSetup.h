
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



#ifndef H_GLOOST_WINDOWSETUP
#define H_GLOOST_WINDOWSETUP



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>



namespace gloost
{

  class WindowWidget;
  class VxfvGui;


  //  Wraps a WindowWidget, loads and saves window configurations including widgets

class WindowSetup
{
	public:

    // class constructor
    WindowSetup(VxfvGui* gui,
               unsigned screenWidth,
               unsigned screenHeight);

    // class destructor
	  virtual ~WindowSetup();


    // init the WindowSetup
	  WindowWidget* getWindowWidget();


	  // frame step
	  virtual void frameStep();

    // resize screen
	  virtual void resize(unsigned screenWidth, unsigned screenHeight);


  protected:





	protected:

   VxfvGui*      _gui;
   WindowWidget* _windowWidget;

   unsigned _screenWidth;
   unsigned _screenHeight;

};


} // namespace gloost


#endif // H_GLOOST_WINDOWSETUP




