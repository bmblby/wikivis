
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


/// ggui system includes
#include <gloost/gui/Gui.h>
#include <gloost/gui/WidgetGroup.h>


// gloost includes
#include <gloost/gl/gloostRenderGoodies.h>



// cpp includes
#include <string>
#include <iostream>
#include <cstring>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

WidgetGroup::WidgetGroup(std::string name,
                         std::string title,
                         gloost::Vector2 position):
    Widget("gloost_widgetgroup", name)
{

  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);






  // properties
  setProp("_title",        title);

}

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

WidgetGroup::WidgetGroup(const std::string& xmlFilePath):
    Widget("loaded from xml", "loaded from xml")
{

  loadResources();

  readXml(xmlFilePath);
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

WidgetGroup::~WidgetGroup()
{
	delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
WidgetGroup::loadResources()
{
  _textMaker = new gloost::TextureText(_gui->getDataFolderPath() +  "/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);

  Vector2 titleDimension = _textMaker->determineLineDimension( getPropString("_title") );
  setProp("_width",  titleDimension[0] + 10);
  setProp("_height", titleDimension[1] + 10);

}


///////////////////////////////////////////////////////////////////////////////


  /// render

void
WidgetGroup::onDraw()
{

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {

    // get the props onces for faster access
    int posX   = getPropInt("_posX");
    int posY   = getPropInt("_posY");

    unsigned int width  = (unsigned int) getPropInt("_width");
    unsigned int height = (unsigned int) getPropInt("_height");



    /// outer Stroke (highlighted)
    if (getPropBool("_mouseWithin") || getPropBool("_mouseDown"))
    {
      glColor4f(0.8f, 0.9f, 0.8f, 1.0);

    }
    else
    {
      glColor4f(0.45f, 0.45f, 0.45f, 1.0);

    }

    {
      glPushMatrix();
      {
        glTranslatef(posX, posY, 0.0);
        glScalef(width, height, 1);
        gl::drawQuad();
      }
      glPopMatrix();
    }


    // draw background
    glColor4f(0.0f, 0.0f, 0.0f, 1.0);
    {
      glPushMatrix();
      {
        glTranslatef(posX+1, posY+1, 0.0);
        glScalef(width-2, height-2, 1.0f);
        gl::drawQuad();
      }
      glPopMatrix();
    }



    /// text
    glColor4f(1.0, 1.0, 1.0, 1.0);

    _textMaker->bindFontTexture();

      /// title
      _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_LEFT);

      _textMaker->renderTextLine(posX,
                                 posY + 3,
                                 getPropString("_title"));

    _textMaker->unbindFontTexture();


  }
  glPopAttrib();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
WidgetGroup::onEnterFrame()
{

  if (getPropBool("_mouseDown"))
  {
    setProp("_posX", getPropFloat("_posX") + _gui->getMouse()->getSpeed()[0]);
    setProp("_posY", getPropFloat("_posY") + _gui->getMouse()->getSpeed()[1]);


    std::list<Widget*>::iterator widgetIt =  _widgetList.begin();
    std::list<Widget*>::iterator endIt    =  _widgetList.end();

    for(; widgetIt != endIt; ++widgetIt)
    {
      Widget* widget = (*widgetIt);

      widget->setProp("_posX", widget->getPropFloat("_posX") + _gui->getMouse()->getSpeed()[0]);
      widget->setProp("_posY", widget->getPropFloat("_posY") + _gui->getMouse()->getSpeed()[1]);

    }


  }


}

///////////////////////////////////////////////////////////////////////////////


/// add a widget to this goup

void
WidgetGroup::addWidget(Widget* widget)
{
  _widgetList.push_back(widget);
}


///////////////////////////////////////////////////////////////////////////////


/// remove a widget from this goup

void
WidgetGroup::removeWidget(Widget* widget)
{
  _widgetList.remove(widget);
}


///////////////////////////////////////////////////////////////////////////////





} // namespace gloost







