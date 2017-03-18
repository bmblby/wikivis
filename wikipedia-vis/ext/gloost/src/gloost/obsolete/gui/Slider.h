
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



#ifndef GLOOST_SLIDER_H
#define GLOOST_SLIDER_H



// gloost includes
#include <gloost/TextureText.h>
#include <gloost/TextLog.h>
#include <gloost/human_input/MouseState.h>
#include <gloost/Vector2.h>


#include <gloost/gui/Widget.h>


// cpp includes
#include <string>



namespace gloost
{


  /// (Gui widget) Slider widget with configurable definition range and type

class Slider: public Widget
{
	public:

    /// class constructor
    Slider(std::string name,
           std::string title,
           gloost::Vector2 position,
           gloost::Vector2 size = gloost::Vector2(100.0f, 18.0f),
           float minValue = 0.0f,
           float maxvalue = 1.0f,
           float defaultValue = 0.0f,
           std::string valueType = "float");

    /// class destructor
	  ~Slider();



    /// loads all necessary resources for this node
    void loadResources();


	  /// render the screen
    void onDraw();

    ///
    /*virtual*/ void onEnterFrame();


    ///
    /*virtual*/ void onMouseUpOutside();


    /// recalc the scaled value
    void normalizedToValue();

    /// recalc the normalized value from value
    void valueToNormalized();

	private:

	  gloost::TextureText* _textMaker;
};


} // namespace ggui


#endif // GLOOST_SLIDER_H






