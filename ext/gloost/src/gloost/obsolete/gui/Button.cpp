
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
#include <gloost/gui/Button.h>


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

Button::Button(std::string name,
               std::string title,
               gloost::Vector2 position,
               gloost::Vector2 size):
    Widget("gloost_button", name)
{
  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);

  // button properties
  setProp("_title",        title);

  setProp("_sizex",        size[0]);
  setProp("_sizey",        size[1]);
}

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Button::Button(const std::string& xmlFilePath):
    Widget("loaded from xml", "loaded from xml")
{
  loadResources();

  readXml(xmlFilePath);
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Button::~Button()
{
	delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
Button::loadResources()
{
  _textMaker = new gloost::TextureText(_gui->getDataFolderPath() +  "/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);

    /// button is always as big as the titletext plus given size
  Vector2 titleDimension = _textMaker->determineLineDimension( getPropString("_title") );
  setProp("_width",  titleDimension[0]+getPropInt("_sizex"));
  setProp("_height", titleDimension[1]+getPropInt("_sizey"));
}

///////////////////////////////////////////////////////////////////////////////


  /// render

void
Button::onDraw()
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
    if (getPropBool("_mouseWithin"))
    {
      gloostColor4v(borderColor1.data());

    }
    else
    {
      gloostColor4v(borderColor0.data());

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




    if (getPropBool("_mouseDown") == true )
    {
      glColor4f(0.45, 0.45f, 0.2f, 1.0);
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
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_CENTER);

    _textMaker->renderTextLine(posX + width/2,
                               posY + height/2  - _textMaker->getCharHeight()/2,
                               getPropString("_title"));


    _textMaker->unbindFontTexture();
  }
  glPopAttrib();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Button::onEnterFrame()
{

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Button::onMouseUp()
{

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Button::onMouseDown()
{

}


///////////////////////////////////////////////////////////////////////////////


  /// ...







///////////////////////////////////////////////////////////////////////////////





} // namespace gloost






