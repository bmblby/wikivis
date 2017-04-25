
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
#include <gloost/gui/TextInput.h>


// gloost includes
#include <gloost/gl/gloostRenderGoodies.h>
#include <gloost/gl/gloostGlUtil.h>


// cpp includes
#include <string>
#include <iostream>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

TextInput::TextInput(std::string name,
                     std::string title,
                     const std::string& defaultText,
                     gloost::Vector2 position,
                     gloost::Vector2 size):
    Widget("gloost_TextInput", name)
{

  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);


  // text input properties
  setProp("_title",        title);
  setProp("_handleKeys",   true);
  setProp("_writeEnable",  false);
  setProp("_inputLength",   50);

  /// button is always as big as the titletext plus given size

  setProp("_value",  defaultText);
  setProp("_sizex",  size[0]);
  setProp("_sizey",  size[1]);
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

TextInput::~TextInput()
{
	delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
TextInput::loadResources()
{
  _textMaker = new gloost::TextureText(_gui->getDataFolderPath() +  "/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);

  Vector2 titleDimension = _textMaker->determineLineDimension( getPropString("_title") );
  setProp("_width",      titleDimension[0]+getPropInt("_sizex"));
  setProp("_confWidth",  titleDimension[0]+getPropInt("_sizex"));
  setProp("_height", titleDimension[1]+getPropInt("_sizey"));
}


///////////////////////////////////////////////////////////////////////////////


  /// render

void
TextInput::onDraw()
{

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {

    // get the props onces for faster access
    int posX   = getPropInt("_posX");
    int posY   = getPropInt("_posY");

    unsigned int width  = (unsigned int) getPropInt("_width");
    unsigned int height = (unsigned int) getPropInt("_height");

    Point3 backgroundColor(0.0f, 0.0f, 0.0f);
    Point3 borderColor0(0.45f, 0.45f, 0.2f);
    Point3 borderColor1(0.75f, 0.7f, 0.4f);



    /// outer Stroke
    std::string cursor;

    if (getPropBool("_writeEnable"))
    {
      gloostColor4v(borderColor1.data());
    }
    else
    {
      gloostColor4v(borderColor0.data());
    }

    glPushMatrix();
    {
      glTranslatef(posX, posY, 0.0);
      glScalef(width, height, 1);
      gloost::gl::drawQuad();
    }
    glPopMatrix();



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


    if (getPropBool("_mouseDown") || getPropBool("_writeEnable") )
    {
      glColor4f(0.40, 0.40f, 0.2f, 1.0);
    }
    else
    {
      glColor4f(0.2, 0.2, 0.1f, 1.0f);
    }


    glPushMatrix();
    {
      glTranslatef(posX+2, posY+2, 0.0);
      glScalef(width-4, height-4, 1.0f);
      gloost::gl::drawQuad();
    }
    glPopMatrix();






    /// text
    glColor4f(1.0, 1.0, 1.0, 1.0);

    _textMaker->bindFontTexture();

    /// title
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_LEFT);

    _textMaker->renderTextLine(posX,
                               posY + height + 3,
                               getPropString("_title"));

    /// content
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_LEFT);

    if (getPropBool("_writeEnable"))
    {
      _textMaker->renderTextLine(posX + _textMaker->getCharWidth()*0.5,
                                 posY + height/2  - _textMaker->getCharHeight()/2,
                                 getPropString("_value")+"|");
      }
    else
    {
      _textMaker->renderTextLine(posX + _textMaker->getCharWidth()*0.5,
                                 posY + height/2  - _textMaker->getCharHeight()/2,
                                 getPropString("_value"));
    }


    _textMaker->unbindFontTexture();
  }
  glPopAttrib();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
TextInput::onEnterFrame()
{

}


///////////////////////////////////////////////////////////////////////////////


  ///

/// handle a incomming char (textinput)

/*virtual*/
bool
TextInput::onHandleKey(const unsigned char& asciiChar)
{
  if (getPropBool("_writeEnable"))
  {
    /// backspace
    if (asciiChar == 8 && getPropString("_value").length())
    {
      setProp("_value", getPropString("_value").substr(0, getPropString("_value").length()-1));
    }
    /// char
    else if (asciiChar >= 32 && asciiChar <= 127 &&
             getPropString("_value").length() <= (unsigned int)getPropInt("_inputLength"))
    {
      setProp("_value", getPropString("_value") + gloost::toString(asciiChar) );
    }
    /// return
    else if (asciiChar == 13)
    {
      setProp("_writeEnable",  false);
    }


    Vector2 titleDimension = _textMaker->determineLineDimension( getPropString("_value") );
    setProp("_width",  math::max((float)titleDimension[0]+_textMaker->getCharWidth(),
             getPropFloat("_confWidth")));


    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////


  ///

///*virtual*/
void
TextInput::onMouseDown()
{
  setProp("_writeEnable",  !getPropBool("_writeEnable"));
}


///////////////////////////////////////////////////////////////////////////////


  ///

///*virtual*/
void
TextInput::onMouseUpOutside()
{
  setProp("_writeEnable",  false);
}


///////////////////////////////////////////////////////////////////////////////


  /// ...







///////////////////////////////////////////////////////////////////////////////





} // namespace gloost







