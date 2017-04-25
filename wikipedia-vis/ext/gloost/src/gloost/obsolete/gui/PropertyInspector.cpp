
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
#include <gloost/gui/PropertyInspector.h>


// gloost includes
#include <gloost/gl/gloostRenderGoodies.h>



// cpp includes
#include <string>
#include <iostream>
#include <map>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

PropertyInspector::PropertyInspector(std::string name,
                                     gloost::Vector2 position,
                                     gloost::Vector2 size):
    Widget("gloost_propertyinspector", name),
    _lastWidgetInspected(0),
    _propertySetToView(0)
{

  loadResources();

  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);
  setProp("_width",  size[0]);
  setProp("_height", size[1]);

  // indicates that the inspector shown information about itself when mouse over
  // this can be confusing if you point with a mouse to a shown value
  setProp("_selfInspect", false);



}


////////////////////////////////////////////////////////////////////////////////


  /// class constructor

PropertyInspector::~PropertyInspector()
{
	delete _textMaker;
}


////////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
PropertyInspector::loadResources()
{
  _textMaker = new gloost::TextureText("../../data/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);
  _textMaker->setLineSpace(-4);
}


////////////////////////////////////////////////////////////////////////////////


  /// set the PropertySet/Widget to inspect

void
PropertyInspector::setPropertySetToView(PropertySet* propset)
{
  _propertySetToView = propset;
}


///////////////////////////////////////////////////////////////////////////////


  /// render

void
PropertyInspector::onDraw()
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
      glColor4f(0.8f, 0.9f, 0.8f, 0.5);

    }
    else
    {
      glColor4f(0.45f, 0.45f, 0.45f, 0.5);

    }

    {
      glPushMatrix();
      {
        glTranslatef(posX, posY, 0.0);
        glScalef(width, height, 1);
        gloost::gl::drawQuad();
      }
      glPopMatrix();
    }


    // draw background
    glColor4f(0.0f, 0.0f, 0.0f, 0.7);
    {
      glPushMatrix();
      {
        glTranslatef(posX+1, posY+1, 0.0);
        glScalef(width-2, height-2, 1.0f);
        gloost::gl::drawQuad();
      }
      glPopMatrix();
    }




    /// text
    glColor4f(1.0, 1.0, 1.0, 1.0);

    _textMaker->bindFontTexture();

    /// title
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_LEFT);

    _textMaker->renderTextLine(posX,
                               posY + height + 3,
                               "PropertyInspector");


    PropertySet* propSet = 0;

    if (_propertySetToView)
    {
      propSet = _propertySetToView;
    }
    else
    {
      propSet = _gui->getWidgetByMousePosition();
    }

    if (propSet && (!getPropBool("_selfInspect") && !(propSet == this ) ))
    {
      _lastWidgetInspected = propSet;
    }

    if (_lastWidgetInspected)
    {

      std::map<std::string, std::string> stringProps = _lastWidgetInspected->getPropsString();

      std::map<std::string, std::string>::const_iterator stringsIt    = stringProps.begin();
      std::map<std::string, std::string>::const_iterator stringsEndIt = stringProps.end();

      glColor4f(0.8f, 1.0f, 0.8f, 1.0);
      _textMaker->renderTextLine(posX+4,
                                 (int) (posY + height - _textMaker->getCharHeight()*1.5),
                                 "String properties:");


      unsigned int counter = 0;

      for( ; stringsIt != stringsEndIt; ++stringsIt)
      {
        if (counter % 2 == 0)
        {
          glColor4f(0.6f, 0.8f, 0.5f, 1.0);
        }
        else
        {
          glColor4f(0.8f, 0.8f, 0.5f, 1.0);
        }
        ++counter;

        _textMaker->renderTextLine( (*stringsIt).first + ": " +  (*stringsIt).second);
      }


      /// string properties
      _textMaker->renderFreeLine();
      _textMaker->renderFreeLine();

      std::map<std::string, float> numProps = _lastWidgetInspected->getProps();

      std::map<std::string, float>::const_iterator numIt    = numProps.begin();
      std::map<std::string, float>::const_iterator numEndIt = numProps.end();

      glColor4f(0.8f, 1.0f, 0.8f, 1.0);
      _textMaker->renderTextLine("Numeric properties:");


      counter = 0;

      for( ; numIt != numEndIt; ++numIt)
      {

        if (counter % 2 == 0)
        {
          glColor4f(0.6f, 0.8f, 0.5f, 1.0);
        }
        else
        {
          glColor4f(0.8f, 0.8f, 0.5f, 1.0);
        }
        ++counter;

        _textMaker->renderTextLine( gloost::toString((*numIt).first)
                                    + ": "
                                    +  gloost::toString((*numIt).second));
      }


      /// Point3 properties
      _textMaker->renderFreeLine();
      _textMaker->renderFreeLine();

      std::map<std::string, gloost::Point3> point3Props = _lastWidgetInspected->getPropsPoint3();

      std::map<std::string, gloost::Point3>::const_iterator p3It    = point3Props.begin();
      std::map<std::string, gloost::Point3>::const_iterator p3ItEnd = point3Props.end();

      glColor4f(0.8f, 1.0f, 0.8f, 1.0);
      _textMaker->renderTextLine("Point3 properties:");


      counter = 0;

      for( ; p3It != p3ItEnd; ++p3It)
      {

        if (counter % 2 == 0)
        {
          glColor4f(0.6f, 0.8f, 0.5f, 1.0);
        }
        else
        {
          glColor4f(0.8f, 0.8f, 0.5f, 1.0);
        }
        ++counter;

        _textMaker->renderTextLine( gloost::toString((*p3It).first)
                                    + ": "
                                    +  gloost::toString((*p3It).second[0])
                                    + ", "
                                    +  gloost::toString((*p3It).second[1])
                                    + ": "
                                    +  gloost::toString((*p3It).second[2]));
      }










    }


    _textMaker->unbindFontTexture();

  }
  glPopAttrib();
}


///////////////////////////////////////////////////////////////////////////////





} // namespace gloost







