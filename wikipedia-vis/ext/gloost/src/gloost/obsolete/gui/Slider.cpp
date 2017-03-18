
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



// gloost includes
#include <gloost/gl/gloostRenderGoodies.h>
#include <gloost/gui/Gui.h>
#include <gloost/gui/Slider.h>



// cpp includes
#include <string>
#include <iostream>
#include <cstring>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Slider::Slider(std::string name,
               std::string title,
               gloost::Vector2 position,
               gloost::Vector2 size,
               float minValue,
               float maxvalue,
               float defaultValue,
               std::string valueType):
    Widget("gloost_slider", name)
{

  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);
  setProp("_width",  size[0]);
  setProp("_height", size[1]);


  // slider properties
  setProp("_title",        title);
  setProp("_normValue",    0.0);
  setProp("_defaultValue", defaultValue);
  setProp("_value",        defaultValue);
  setProp("_minValue",     minValue);
  setProp("_maxValue",     maxvalue);
  setProp("_valueType",    valueType);    /// can be "bool", "int" and "float"


  // calc the normalized value for the given default value
  valueToNormalized();

  // map value to the valuetype
  normalizedToValue();
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Slider::~Slider()
{
	delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
Slider::loadResources()
{
  _textMaker = new gloost::TextureText( _gui->getDataFolderPath() +  "/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);
}


///////////////////////////////////////////////////////////////////////////////


  /// render

void
Slider::onDraw()
{

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {

    // get the props onces for faster access
    int posX   = getPropInt("_posX");
    int posY   = getPropInt("_posY");

    unsigned int width  = (unsigned int) getPropInt("_width");
    unsigned int height = (unsigned int) getPropInt("_height");



    /// outer Stroke
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
        gloost::gl::drawQuad();
      }
      glPopMatrix();
    }


    // draw slider background
    glColor4f(0.0f, 0.0f, 0.0f, 1.0);
    {
      glPushMatrix();
      {
        glTranslatef(posX+1, posY+1, 0.0);
        glScalef(width-2, height-2, 1.0f);
        gloost::gl::drawQuad();
      }
      glPopMatrix();
    }


    glColor4f(0.26f, 0.33f, 0.2f, 1.0f);
    float valueToPosition = getPropFloat("_normValue") * (width - 4);

    {
      glPushMatrix();
      {
        glTranslated(posX + 2,
                     posY + 2,
                     0.0f);
        glScalef(valueToPosition, height-4, 1.0f);
        gloost::gl::drawQuad();
      }
      glPopMatrix();
    }



    /// draw value bar tip
    glColor4f(0.4f, 0.8f, 0.4f, 1.0f);

    {
      glPushMatrix();
      {
        glTranslated(posX + valueToPosition + 1,
                     posY + 2,
                     0.0f);
        glScalef(2, height-4, 1.0f);
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
                                 getPropString("_title"));


      /// value
      _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_CENTER);

      _textMaker->renderTextLine(posX + width/2,
                                 posY + height/2 - _textMaker->getCharHeight()/2,
                                 getPropFloatAsString("_value"));

    _textMaker->unbindFontTexture();


  }
  glPopAttrib();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Slider::onEnterFrame()
{

  if (getPropBool("_mouseDown"))
  {
    gloost::human_input::MouseState* mouse = _gui->getMouse();

    float value = (mouse->getLoc()[0] - getPropFloat("_posX") - 2) / (getPropFloat("_width") -4);

    if (value < 0.0f)
    {
      value = 0.0f;
    }
    if (value > 1.0f)
    {
      value = 1.0f;
    }

    setProp("_normValue", value);

  }

  normalizedToValue();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Slider::onMouseUpOutside()
{

//  gloost::TextLog::getInstance()->addLine("GuiNode::mouseUpOutside(): _normValue = " + getPropFloatAsString("_normValue"),
//                                           gloost::vec3(0.8, 0.2, 0.2));
//
//  gloost::TextLog::getInstance()->addLine();

}


///////////////////////////////////////////////////////////////////////////////


  /// recalc the value from the normalized value

void
Slider::normalizedToValue()
{
  float valueWidth = getPropFloat("_maxValue") - getPropFloat("_minValue");
  setProp("_value", getPropFloat("_normValue") * valueWidth + getPropFloat("_minValue"));


  /// convert to valuetype int or bool
  std::string valueType = getPropString("_valueType");

  if (std::strcmp(valueType.c_str(), "int") == 0)
  {
    setProp("_value", getPropInt("_value"));
  }

  else if (std::strcmp(valueType.c_str(), "bool") == 0)
  {
    setProp("_value", getPropBool("_value"));
  }

}


///////////////////////////////////////////////////////////////////////////////


  /// recalc the normalized value from value

void
Slider::valueToNormalized()
{
  float valueWidth = getPropFloat("_maxValue") - getPropFloat("_minValue");
  setProp("_normValue", (getPropFloat("_value") - getPropFloat("_minValue")) / valueWidth);
}


///////////////////////////////////////////////////////////////////////////////


  /// ...







///////////////////////////////////////////////////////////////////////////////





} // namespace ggui






