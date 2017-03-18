
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



#ifndef GLOOST_PROPERTYINSPECTOR_H
#define GLOOST_PROPERTYINSPECTOR_H



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


  /// (Gui widget) Shows all properties of other widgets on mouseover

class PropertyInspector: public Widget
{
	public:

    /// class constructor
    PropertyInspector(std::string name,
                      gloost::Vector2 position,
                      gloost::Vector2 size = gloost::Vector2(180.0f, 200.0f));

    /// class destructor
	  ~PropertyInspector();


    /// set the PropertySet/Widget to inspect
    void setPropertySetToView(PropertySet* propset = 0);


    /// loads all necessary resources for this node
    void loadResources();


	  /// render the screen
    void onDraw();

	private:

	  gloost::TextureText* _textMaker;

	  PropertySet* _lastWidgetInspected;
	  PropertySet* _propertySetToView;

};


} // namespace gloost


#endif // GLOOST_PROPERTYINSPECTOR_H








