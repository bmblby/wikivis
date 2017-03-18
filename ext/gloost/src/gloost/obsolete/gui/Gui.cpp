
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
#include <gloost/TextLog.h>



/// ggui system includes
#include <gloost/gui/Gui.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{


/**
  \class Gui

  \brief Gui handles mouse events, holds and handles Widget objects like button, sliders, toggles and more.

  \author Felix Weiszig
  \date   Januar 2010

  \remarks Here some hints how to use the Gui with glut windows:
           <br/>
           <br/>
           <b>glut:</b><br/>
           \code

           // global
           gloost::Gui* g_gui = 0;

           void init()
           {
             // create a Gui with the path to the data (usualy font textures)
             g_gui = new gloost::Gui("../../data/");
           }

           void motionFunc(int mouse_h, int mouse_v)
           {
             g_gui->glutMotionFunc(mouse_h, g_screenHeight-mouse_v);
           }

           void mouseFunc(int button, int state, int mouse_h, int mouse_v)
           {
             g_gui->glutMouseFunc(button, state, mouse_h, g_screenHeight-mouse_v);
           }

           int main(int argc, char *argv[])
           {
             ...
             // route the glut mouse callbacks like this:
             glutMotionFunc(motionFunc);
             glutPassiveMotionFunc(motionFunc);
             glutMouseFunc(mouseFunc);
             ...
           }
           \endcode
*/


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

/*private*/
Gui::Gui(const std::string& dataFolderPath, gloost::human_input::MouseState* mouse):
  _guiElements(),
  _mouse(mouse),
  _mouseIsExtern(0),
  _guiWidth(3000),
  _guiHeight(3000),
  _dataFolderPath(dataFolderPath),
  _idCounter(0),
  _widgetsById(),
  _widgetsByName(),
  _currentActiveWidget(0),
  _enabled(1)
{
  /// create  mouse
  if (!_mouse)
  {
    _mouse = new gloost::human_input::MouseState();
  }
  else
  {
    _mouseIsExtern = true;
  }

}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Gui::~Gui()
{
  WidgetListIterator nodeIt = _guiElements.begin();
  WidgetListIterator endIt  = _guiElements.end();

  while( nodeIt != endIt )
  {
    nodeIt = _guiElements.erase(nodeIt) ;
  }
}


///////////////////////////////////////////////////////////////////////////////


  ///

bool
Gui::frameStep()
{
  /// indicates that the mouse is inside a gui widget
  bool guiHit = false;


  // call per-frame events on the tree
  if (_enabled)
  {

    /// call enterFrame on all widgets to do dynamic stuff
    {
      std::list<Widget*>::const_iterator nodeIt = _guiElements.begin();
      std::list<Widget*>::const_iterator endIt  = _guiElements.end();

      for( ; nodeIt != endIt; ++nodeIt)
      {
        (*nodeIt)->enterFrame();
      }
    }


    /// this will hapen
    if (_mouse->getButtonUpEvent(GLOOST_MOUSESTATE_BUTTON1) && !getWidgetByMousePosition())
    {
      callMouseUpOutsideOnAllWidgets();
    }

    /// let the widget handle the mouse to create events
    {
      std::list<Widget*>::const_iterator nodeIt = _guiElements.begin();
      std::list<Widget*>::const_iterator endIt  = _guiElements.end();

      for( ; nodeIt != endIt; ++nodeIt)
      {
        bool guiHitNow = false;
        guiHitNow = (*nodeIt)->handleMouse(_mouse);

        if (guiHitNow)
        {
          _currentActiveWidget =  (*nodeIt);
        }

        guiHit = guiHit || guiHitNow;


      }
    }


    /// call exitFrame on all widgets
    {
      std::list<Widget*>::const_iterator nodeIt = _guiElements.begin();
      std::list<Widget*>::const_iterator endIt  = _guiElements.end();

      for( ; nodeIt != endIt; ++nodeIt)
      {
        (*nodeIt)->exitFrame();
      }
    }


    // reset the mouse for the next frame
    if (!_mouseIsExtern)
    {
      getMouse()->setSpeedToZero();
      getMouse()->resetMouseEvents();
    }
  }

  return guiHit;
}


///////////////////////////////////////////////////////////////////////////////


  /// draw this gui

void
Gui::draw()
{

  glPushMatrix();
    {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      {

        glViewport(0, 0, _guiWidth, _guiHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, (float) _guiWidth, 0, (float) _guiHeight, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 50.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);

        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_DEPTH_TEST);

        glDisable(GL_DEPTH_TEST);

        glDisable(GL_LIGHTING);

        // color
        glColor4d(1.0f, 1.0f, 1.0f, 1.0f);


        // draw all gui elements
        if (_guiElements.size() != 0)
        {
          if (_enabled)
          {
            std::list<Widget*>::const_iterator nodeIt = _guiElements.begin();
            std::list<Widget*>::const_iterator endIt  = _guiElements.end();

            for( ; nodeIt != endIt; ++nodeIt)
            {
              (*nodeIt)->draw();
            }
          }
        }
    }
    glPopAttrib();
  }
  glPopMatrix();
}


///////////////////////////////////////////////////////////////////////////////


  /// get the window (or one of the children of the root node) below the mouse pointer

Widget*
Gui::getWidgetByMousePosition()
{
  std::list<Widget*>::iterator index = _guiElements.begin();
  std::list<Widget*>::iterator end   = _guiElements.end();

  Widget* hitNode   = 0;

  for( ; index != end; ++index)
  {
    Widget* node = (*index);

    if (node->isMouseInside(_mouse->getLoc()))
    {
      hitNode = node;
    }
  }

  return hitNode;
}


///////////////////////////////////////////////////////////////////////////////


  /// register a Widget to find it in the tree for communication

unsigned int
Gui::addWidget(Widget* widget)
{
  widget->setGui(this);
  widget->loadResources();;

  _guiElements.push_back(widget);

  /// register node to id lookup
  unsigned int newId = getNewUniqueId();
  _widgetsById[newId] = widget;

  /// register to name lookup
  _widgetsByName[widget->getPropString("_name")] = widget;

  return newId;
}


///////////////////////////////////////////////////////////////////////////////


  /// set a property on all widgets

void
Gui::setPropOnAllWidgets(std::string name, float value)
{
  WidgetListConstIterator nodeIt = _guiElements.begin();
  WidgetListConstIterator endIt  = _guiElements.end();

  for( ; nodeIt != endIt; ++nodeIt)
  {
    (*nodeIt)->setProp(name, value);
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// set a property on all widgets

void
Gui::setPropOnAllWidgets(std::string name, std::string value)
{
  WidgetListConstIterator nodeIt = _guiElements.begin();
  WidgetListConstIterator endIt  = _guiElements.end();

  for( ; nodeIt != endIt; ++nodeIt)
  {
    (*nodeIt)->setProp(name, value);
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// move all widgets

void
Gui::moveAllWidgets(const Vector2& offset)
{
  std::list<Widget*>::const_iterator nodeIt = _guiElements.begin();
  std::list<Widget*>::const_iterator endIt  = _guiElements.end();

  for( ; nodeIt != endIt; ++nodeIt)
  {
    (*nodeIt)->setProp("_posX", (*nodeIt)->getPropFloat("_posX") + offset[0]);
    (*nodeIt)->setProp("_posY", (*nodeIt)->getPropFloat("_posY") + offset[1]);
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// call mouseUpOutside on all widgets

void
Gui::callMouseUpOutsideOnAllWidgets()
{
  std::list<Widget*>::const_iterator nodeIt = _guiElements.begin();
  std::list<Widget*>::const_iterator endIt  = _guiElements.end();

  for( ; nodeIt != endIt; ++nodeIt)
  {
    (*nodeIt)->mouseUpOutside();
  }
}



///////////////////////////////////////////////////////////////////////////////


  ///

unsigned int
Gui::getNewUniqueId()
{
	++_idCounter;
	return _idCounter;
}


///////////////////////////////////////////////////////////////////////////////


  /// get a registered Widget by Id

Widget*
Gui::getWidgetById(unsigned int id)
{
  std::map<unsigned int, Widget*>::iterator pos = _widgetsById.find(id);

  if (pos != _widgetsById.end())
  {
    return (*pos).second;
  }

  std::cout << std::endl;
  std::cout << std::endl << "WARNING from Gui::getWidgetByName(std::string name):";
  std::cout << std::endl << "             Could NOT find widget with id " << id << "!";
  std::cout << std::endl;

  return 0;
}


///////////////////////////////////////////////////////////////////////////////


  /// get a registered Node by Name

Widget*
Gui::getWidgetByName(const std::string& name)
{
  std::map<std::string, Widget*>::iterator pos = _widgetsByName.find(name);

  if (pos != _widgetsByName.end())
  {
    return (*pos).second;
  }

  std::cout << std::endl;
  std::cout << std::endl << "WARNING from Gui::getWidgetByName(std::string name):";
  std::cout << std::endl << "             Could NOT find widget with name \"" << name << "\"!";
  std::cout << std::endl;

  return 0;
}


///////////////////////////////////////////////////////////////////////////////


  /// current active widget

Widget*
Gui::getCurrentActiveWidget()
{
  return _currentActiveWidget;
}


///////////////////////////////////////////////////////////////////////////////


  /// set current active widget

void
Gui::setCurrentActiveWidget(Widget* widget)
{
  _currentActiveWidget = widget;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns the last id given to a widget by the gui

unsigned int
Gui::getLastWidgetId() const
{
  return _idCounter;
}


///////////////////////////////////////////////////////////////////////////////


  /// get a list of type gloost::Gui

Gui::WidgetList*
Gui::getWidgets()
{
  return &_guiElements;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns the path to folder where folders with fonts and textures for the widgets are located

const std::string&
Gui::getDataFolderPath()
{
	return _dataFolderPath;
}


///////////////////////////////////////////////////////////////////////////////


  /// get a pointer to the mouse

gloost::human_input::MouseState*
Gui::getMouse()
{
	return _mouse;
}

///////////////////////////////////////////////////////////////////////////////



void
Gui::glutMotionFunc(int mouse_h, int screenHeightMinusMouse_v)
{

  // set the mouse
  _mouse->setLoc(mouse_h, screenHeightMinusMouse_v);
}


//////////////////////////////////////////////////////////////////////////////////////////


void
Gui::glutMouseFunc(int button, int state, int mouse_h, int screenHeightMinusMouse_v)
{


  getMouse()->setButtonState(button+1, !state);


//
//	if (button == GLUT_LEFT_BUTTON)
//	{
//		if (state == GLUT_DOWN)
//		{
//			getMouse()->setButtonState(GLOOST_MOUSESTATE_BUTTON1, true);
//		}
//		else if (state == GLUT_UP)
//		{
//			getMouse()->setButtonState(GLOOST_MOUSESTATE_BUTTON1, false);
//		}
//	}
//	else if (button == GLUT_RIGHT_BUTTON)
//	{
//		if (state == GLUT_DOWN)
//		{
//			getMouse()->setButtonState(GLOOST_MOUSESTATE_BUTTON2, true);
//		}
//		else
//		{
//			getMouse()->setButtonState(GLOOST_MOUSESTATE_BUTTON2, false);
//		}
//	}
//	else if (button == GLUT_MIDDLE_BUTTON)
//	{
//		if (state == GLUT_DOWN)
//		{
//			getMouse()->setButtonState(GLOOST_MOUSESTATE_BUTTON3, true);
//		}
//		else
//		{
//			getMouse()->setButtonState(GLOOST_MOUSESTATE_BUTTON3, false);
//		}
//	}
}



///////////////////////////////////////////////////////////////////////////////


  /// send a key to all widgets

bool
Gui::handleKeyDownEvent(const unsigned char& asciiChar)
{
  WidgetListIterator widgetIt = _guiElements.begin();
  WidgetListIterator endIt    = _guiElements.end();

  bool received = false;

  for(; widgetIt != endIt; ++widgetIt)
  {
    received = received || (*widgetIt)->handleKey(asciiChar);
  }

  return received;
}


///////////////////////////////////////////////////////////////////////////////


  /// enable dragMode on all widgets

void
Gui::enableDragMode()
{
  WidgetListIterator widgetIt = _guiElements.begin();
  WidgetListIterator endIt    = _guiElements.end();

  for(; widgetIt != endIt; ++widgetIt)
  {
    (*widgetIt)->setProp("_dragMode", true);
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// disable dragMode on all widgets

void
Gui::disableDragMode()
{
  WidgetListIterator widgetIt = _guiElements.begin();
  WidgetListIterator endIt    = _guiElements.end();

  for(; widgetIt != endIt; ++widgetIt)
  {
    (*widgetIt)->setProp("_dragMode", false);
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// inverse dragMode on all widgets

void
Gui::toggleDragMode()
{
  WidgetListIterator widgetIt = _guiElements.begin();
  WidgetListIterator endIt    = _guiElements.end();

  for(; widgetIt != endIt; ++widgetIt)
  {
    (*widgetIt)->setProp("_dragMode", !(*widgetIt)->getPropBool("_dragMode"));
  }
}


///////////////////////////////////////////////////////////////////////////////



/// writes the gui to an xml file

bool writeGuiToXml(const std::string& filepath)
{

  return true;
}


///////////////////////////////////////////////////////////////////////////////

} // namespace gloost


