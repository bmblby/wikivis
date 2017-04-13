
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



#ifndef GLOOST_WIDGETGROUP_H
#define GLOOST_WIDGETGROUP_H



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


  /// (Gui widget) Groups and moves other widgets

class WidgetGroup: public Widget
{
	public:

    /// class constructor
    WidgetGroup( std::string name,
                 std::string title,
                 gloost::Vector2 position);

    /// load a xml file
    WidgetGroup(const std::string& xmlFilePath);

    /// class destructor
	  ~WidgetGroup();



    /// loads all necessary resources for this node
    void loadResources();


	  /// render the screen
    void onDraw();

    ///
    /*virtual*/ void onEnterFrame();


    /// add a widget to this goup
    void addWidget(Widget* widget);

    /// remove a widget from this goup
    void removeWidget(Widget* widget);








	private:

	  TextureText*         _textMaker;
	  std::list<Widget*> _widgetList;


};


} // namespace gloost


#endif // GLOOST_PAD_H








