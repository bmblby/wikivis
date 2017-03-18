
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
#include <gloost/gui/Gui.h>
#include <gloost/gui/Widget.h>



namespace gloost
{


///////////////////////////////////////////////////////////////////////////////


  ///

Widget::Widget(std::string type, std::string name):
    PropertySet()
{

  setProp("_type",   type);
  setProp("_name",   name);


  // event related props
  setProp("_mouseWithin",        false);
  setProp("_mouseLeaveEvent",    false);     /// indicates that the mouse left the widget last frame
  setProp("_mouseEnterEvent",    false);     /// indicates that the mouse left the widget last frame
  setProp("_mouseDown",          false);
  setProp("_mouseUpEvent",      false);  /// indicates that the button was pressed last frame


  // customizable events
  setProp("_visible",     true);
  setProp("_handleMouse", true);
  setProp("_handleKeys",  false);


  // dragging
  setProp("_dragMode", false); /// the widget shows the drag marker
  setProp("_drag",     false); /// the widget foolows the mouse

}

///////////////////////////////////////////////////////////////////////////////


 /// destructor

/*virtual*/
Widget::~Widget()
{


}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
Widget::loadResources()
{


}



///////////////////////////////////////////////////////////////////////////////


  /// check the a point if it's inside the rect of this node

bool
Widget::isMouseInside(const gloost::Point3& loc)
{
  // inside
  if ((getPropInt("_posX") < loc[0]) && (getPropInt("_posX")+getPropInt("_width") > loc[0]))
  {
    if ((getPropInt("_posY") < loc[1]) && (getPropInt("_posY")+getPropInt("_height") > loc[1]))
    {
      return true;
    }
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////


  /// main event creating method

bool
Widget::handleMouse(gloost::human_input::MouseState* mouse)
{

  /// if this node don't need the mouse just pass the mouse to the childs
  if (!getPropBool("_handleMouse"))
  {
    return 0;
  }


  // first we check if the mouse position is inside the rect
  bool inside = 0;


  // inside x
  if (isMouseInside(mouse->getLoc()))
  {
    inside = true;
  }


  if (inside)
  {
    /// mouseEnter
    if (!getPropBool("_mouseWithin"))
    {
      mouseEnter();
    }


    /// mouseDown
    if (!getPropBool("_mouseDown") && mouse->getButtonDownEvent(GLOOST_MOUSESTATE_BUTTON1))
    {
      mouseDown();
    }


    /// mouseUp
    if (mouse->getButtonUpEvent(GLOOST_MOUSESTATE_BUTTON1))
    {
      mouseUp();
    }

  }
  else
  {
    /// mouseLeave
    if (getPropBool("_mouseWithin"))
    {
      mouseLeave();
    }

//    /// mouseUpOutside
//    if (mouse->getButtonUpEvent(GLOOST_MOUSESTATE_BUTTON1))
//    {
//      mouseUpOutside();
//    }
  }

  return inside;
}


///////////////////////////////////////////////////////////////////////////////


  /// draw this widget

void
Widget::draw()
{
  if (getPropBool("_visible"))
  {
    onDraw();
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// render this particular node

/*virtual*/
void
Widget::onDraw()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  /// handle a incomming char (textinput)

bool
Widget::handleKey(const unsigned char& asciiChar)
{
  if (getPropBool("_handleKeys"))
  {
    return onHandleKey(asciiChar);
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////////


  /// handle a incomming char (textinput)

bool
Widget::onHandleKey(const unsigned char& asciiChar)
{

  ///
  return 0;

}


/////////////////////////////////////////////////////////////////////////////////
//
//
//  /// get the id of this node
//
//unsigned int
//Widget::getId()
//{
//  return (unsigned int)getPropInt("_id");
//}


///////////////////////////////////////////////////////////////////////////////


  ///


void
Widget::mouseDown()
{

  if (getPropBool("_dragMode") == false)
  {

    setProp("_mouseDown", true);
    onMouseDown();
  }
  else
  {
    setProp("_drag", true);
  }
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onMouseDown()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::mouseUp()
{

  if (getPropBool("_drag") == false)
  {

    setProp("_mouseDown", false);
    setProp("_mouseUpEvent", true);
    onMouseUp();
  }
  else
  {
    setProp("_drag", false);
  }
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onMouseUp()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::mouseUpOutside()
{

  if (getPropBool("_drag") == false)
  {
    if (!getPropBool("_mouseWithin")/* && getPropBool("_mouseDown")*/)
    {
      setProp("_mouseDown", false);
//      setProp("_mouseUpEvent", true);


        onMouseUpOutside();
      }
   }
   else
   {
    setProp("_drag", false);
  }
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onMouseUpOutside()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::mouseEnter()
{

  setProp("_mouseWithin", true);
  setProp("_mouseEnterEvent",    true);
  onMouseEnter();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onMouseEnter()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::mouseLeave()
{

  if (!getPropBool("_mouseWithin"))
  {
    return;
  }

  setProp("_mouseWithin",     false);
  setProp("_mouseLeaveEvent", true);

  onMouseLeave();
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onMouseLeave()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::mouseWithin()
{
  onMouseWithin();
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onMouseWithin()
{

  /// implement this function in your derived class ...

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::enterFrame()
{

  if (getPropBool("_drag") == false)
  {

    if (getPropBool("_mouseWithin") == true)
    {
      mouseWithin();
    }

    setProp("_mouseUpEvent",    false);
    setProp("_mouseLeaveEvent", false);
    setProp("_mouseEnterEvent", false);

    onEnterFrame();

  }
  else
  {

    /// dragging
    Vector3 mouseSpeed = _gui->getMouse()->getSpeed();

    setProp("_posX", getPropFloat("_posX") + mouseSpeed[0]);
    setProp("_posY", getPropFloat("_posY") + mouseSpeed[1]);
  }

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onEnterFrame()
{



}


///////////////////////////////////////////////////////////////////////////////


  ///

void
Widget::exitFrame()
{
  onExitFrame();
}


///////////////////////////////////////////////////////////////////////////////


  /// gui object uses this methode to add a pointer to this widget

void
Widget::setGui(Gui* gui)
{
  _gui = gui;
//  std::cout << std::endl << " word : " << getPropString("_name");
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Widget::onExitFrame()
{

  /// enter code here ...

}


///////////////////////////////////////////////////////////////////////////////










} // namespace gloost

