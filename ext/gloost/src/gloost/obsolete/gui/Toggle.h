
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



#ifndef GLOOST_TOGGLE_H
#define GLOOST_TOGGLE_H



// gloost system includes
#include <gloost/TextureText.h>
#include <gloost/TextLog.h>
#include <gloost/human_input/MouseState.h>
#include <gloost/Vector2.h>
#include <gloost/gui/Widget.h>



// cpp includes
#include <string>



namespace gloost
{


  /// (Gui widget) Checkbox/toggle widget to indicate or set a bool value

class Toggle: public Widget
{
	public:

    /// class constructor
    Toggle(std::string name,
           std::string title,
           gloost::Vector2 position,
           gloost::Vector2 size = gloost::Vector2(14.0f, 14.0f),
           bool value = true);

    /// class destructor
	  ~Toggle();



    /// loads all necessary resources for this node
    void loadResources();


	  /// render the screen
    void onDraw();

    ///
    /*virtual*/ void onEnterFrame();


    ///
    /*virtual*/ void onMouseUp();
    /*virtual*/ void onMouseUpOutside();
    /*virtual*/ void onMouseDown();

	private:

	  gloost::TextureText* _textMaker;
};


} // namespace gloost


#endif // GLOOST_TOGGLE_H






