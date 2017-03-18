
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
#include <gloost/gui/Toggle.h>


// gloost includes
#include <gloost/gl/gloostRenderGoodies.h>



// cpp includes
#include <string>
#include <iostream>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Toggle::Toggle(std::string name,
               std::string title,
               gloost::Vector2 position,
               gloost::Vector2 size,
               bool value):
    Widget("gloost_toggle", name)
{

  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);
  setProp("_width",  size[0]);
  setProp("_height", size[1]);


  // toggle properties
  setProp("_title",        title);
  setProp("_value",        value);
  setProp("_wasDownHere",  false);
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Toggle::~Toggle()
{
	delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
Toggle::loadResources()
{
  _textMaker = new gloost::TextureText(_gui->getDataFolderPath() +  "/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);
}


///////////////////////////////////////////////////////////////////////////////


  /// render

void
Toggle::onDraw()
{

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {

    // get the props onces for faster access
    int posX   = getPropInt("_posX");
    int posY   = getPropInt("_posY");

    unsigned int width  = (unsigned int) getPropInt("_width");
    unsigned int height = (unsigned int) getPropInt("_height");


    /// outer Stroke
    if (getPropBool("_mouseWithin"))
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


    // draw slider background
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


    if (getPropBool("_value") == true )
    {
      glColor4f(0.5f, 0.7f, 0.4f, 1.0f);
      {
        glPushMatrix();
        {
          glTranslatef(posX+2, posY+2, 0.0);
          glScalef(width-4, height-4, 1.0f);
          gl::drawQuad();
        }
        glPopMatrix();
      }
    }


    /// text
    glColor4f(1.0, 1.0, 1.0, 1.0);

    _textMaker->bindFontTexture();

    /// title
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_LEFT);

    _textMaker->renderTextLine(posX + width + 5,
                               posY,
                               getPropString("_title"));


    _textMaker->unbindFontTexture();
  }
  glPopAttrib();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Toggle::onEnterFrame()
{


}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Toggle::onMouseUp()
{
  if (getPropBool("_wasDownHere"))
  {
    setProp("_value", !getPropBool("_value"));
    setProp("_wasDownHere", false);
  }
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Toggle::onMouseUpOutside()
{
  setProp("_wasDownHere", false);
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Toggle::onMouseDown()
{
  setProp("_wasDownHere", true);
}


///////////////////////////////////////////////////////////////////////////////


  /// ...







///////////////////////////////////////////////////////////////////////////////





} // namespace ggui






