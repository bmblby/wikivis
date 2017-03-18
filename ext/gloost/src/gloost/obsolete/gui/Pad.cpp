
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
#include <gloost/gui/Pad.h>


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

Pad::Pad(std::string name,
         std::string title,
         gloost::Vector2 position,
         gloost::Vector2 size,
         float minValueX,
         float maxvalueX,
         float defaultValueX ,
         float minValueY,
         float maxvalueY,
         float defaultValueY,
         std::string valueType):
    Widget("gloost_pad", name)
{

  loadResources();

  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);
  setProp("_width",  size[0]);
  setProp("_height", size[1]);


  // properties
  setProp("_title",        title);

  setProp("_normValueX",    0.0);
  setProp("_defaultValueX", defaultValueX);
  setProp("_valueX",        defaultValueX);
  setProp("_minValueX",     minValueX);
  setProp("_maxValueX",     maxvalueX);

  setProp("_normValueY",    0.0);
  setProp("_defaultValueY", defaultValueY);
  setProp("_valueY",        defaultValueY);
  setProp("_minValueY",     minValueY);
  setProp("_maxValueY",     maxvalueY);

  setProp("_markerSize",     12); // size of the marker

  setProp("_grid1_subdivX",     20); // subdivision of the finest grid
  setProp("_grid1_subdivY",     20); // subdivision of the finest grid
  setProp("_grid2_subdivX",     4); // subdivision of the medium grid
  setProp("_grid2_subdivY",     4); // subdivision of the medium grid
  setProp("_grid3_subdivX",     2); // subdivision of the big grid
  setProp("_grid3_subdivY",     2); // subdivision of the big grid


//  setProp("_smoothness", 1.0); /// factor for interpolation of the position


  setProp("_valueType",    valueType);    /// can be "bool", "int" and "float"


  // calc the normalized value for the given default value
  valueToNormalized();

  // map value to the valuetype
  normalizedToValue();
}

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Pad::Pad(const std::string& xmlFilePath):
    Widget("loaded from xml", "loaded from xml")
{

  loadResources();

  readXml(xmlFilePath);

  // calc the normalized value for the given default value
  valueToNormalized();

  // map value to the valuetype
  normalizedToValue();
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Pad::~Pad()
{
	delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// loads all necessary resources for this node

void
Pad::loadResources()
{
  _textMaker = new gloost::TextureText("../../data/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);
}


///////////////////////////////////////////////////////////////////////////////


  /// render

void
Pad::onDraw()
{

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {

    // get the props onces for faster access
    int posX   = getPropInt("_posX");
    int posY   = getPropInt("_posY");

    unsigned int width  = (unsigned int) getPropInt("_width");
    unsigned int height = (unsigned int) getPropInt("_height");

    unsigned int grid1_subX = getPropInt("_grid1_subdivX");
    unsigned int grid1_subY = getPropInt("_grid1_subdivY");
    unsigned int grid2_subX = getPropInt("_grid2_subdivX");
    unsigned int grid2_subY = getPropInt("_grid2_subdivY");
    unsigned int grid3_subX = getPropInt("_grid3_subdivX");
    unsigned int grid3_subY = getPropInt("_grid3_subdivY");


    /// point of interest
    float pointSize = getPropFloat("_markerSize");


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
        gloost::gl::drawQuad();
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
        gloost::gl::drawQuad();
      }
      glPopMatrix();
    }


    /// grid
    glColor4f(0.26f, 0.33f, 0.2f, 0.6f);

    {
      glPushMatrix();
      {
        glTranslated(posX + width/2,
                     posY + height/2,
                     0.0f);
        glScalef(width-pointSize, height-pointSize, 1.0f);

        gloost::gl::drawWireRaster(grid1_subX, grid1_subY);

        gloost::gl::drawWireRaster(grid2_subX, grid2_subY);

        glLineWidth(2.0);
        gloost::gl::drawWireRaster(grid3_subX, grid3_subY);
      }
      glPopMatrix();
    }



    /// draw marker
    int valueToPositionX = (int)(getPropFloat("_normValueX") * (width - pointSize) + pointSize/2);
    int valueToPositionY = (int)(getPropFloat("_normValueY") * (height - pointSize) + pointSize/2);

    {
      glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
      glPushMatrix();
      {
        glTranslated(posX + valueToPositionX,
                     posY + valueToPositionY,
                     0.0f);
        glScalef(pointSize, pointSize, 1.0f);
        gloost::gl::drawQuadCenter();
      }
      glPopMatrix();

      glColor4f(0.4f, 0.8f, 0.4f, 0.6f);
      glPushMatrix();
      {
        glTranslated(posX + valueToPositionX,
                     posY + valueToPositionY,
                     0.0f);
        glScalef(pointSize-2, pointSize-2, 1.0f);
        gloost::gl::drawQuadCenter();
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
      _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_RIGHT);

      _textMaker->renderTextLine(posX + width - 8,
                                 posY + _textMaker->getCharHeight()/2,
                                 "(" + getPropFloatAsString("_valueX") + ", "
                                 + getPropFloatAsString("_valueY") + ")");

    _textMaker->unbindFontTexture();


  }
  glPopAttrib();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
Pad::onEnterFrame()
{

  if (getPropBool("_mouseDown"))
  {
    gloost::human_input::MouseState* mouse = _gui->getMouse();

    float valueX = (mouse->getLoc()[0] - getPropFloat("_posX") ) / (getPropFloat("_width"));

    if (valueX < 0.0f)
    {
      valueX = 0.0f;
    }
    if (valueX > 1.0f)
    {
      valueX = 1.0f;
    }

    setProp("_normValueX", valueX);



    float valueY = (mouse->getLoc()[1] - getPropFloat("_posY")) / (getPropFloat("_height"));

    if (valueY < 0.0f)
    {
      valueY = 0.0f;
    }
    if (valueY > 1.0f)
    {
      valueY = 1.0f;
    }

    setProp("_normValueY", valueY);
  }
//  else
//  {
//    /// travel back to default value
//
//    float backTravelSpeed = getPropFloat("_backToZero");
//
//    if (backTravelSpeed > 0)
//    {
//      setProp("_normValueX", getPropFloat("_defaultValueX"));
//      setProp("_normValueY", getPropFloat("_defaultValueY"));
//    }
//  }

  normalizedToValue();


}


///////////////////////////////////////////////////////////////////////////////


  /// recalc the value from the normalized value

void
Pad::normalizedToValue()
{
  float valueWidthX = getPropFloat("_maxValueX") - getPropFloat("_minValueX");
  setProp("_valueX", getPropFloat("_normValueX") * valueWidthX + getPropFloat("_minValueX"));

  float valueWidthY = getPropFloat("_maxValueY") - getPropFloat("_minValueY");
  setProp("_valueY", getPropFloat("_normValueY") * valueWidthY + getPropFloat("_minValueY"));


  /// convert to valuetype int or bool
  std::string valueType = getPropString("_valueType");

  if (std::strcmp(valueType.c_str(), "int") == 0)
  {
    setProp("_valueX", getPropInt("_valueX"));
    setProp("_valueY", getPropInt("_valueY"));
  }

  else if (std::strcmp(valueType.c_str(), "bool") == 0)
  {
    setProp("_valueX", getPropBool("_valueX"));
    setProp("_valueY", getPropBool("_valueY"));
  }

}


///////////////////////////////////////////////////////////////////////////////


  /// recalc the normalized value from value

void
Pad::valueToNormalized()
{
  float valueWidthX = getPropFloat("_maxValueX") - getPropFloat("_minValueX");
  setProp("_normValueX", (getPropFloat("_valueX") - getPropFloat("_minValueX")) / valueWidthX);

  float valueWidthY = getPropFloat("_maxValueY") - getPropFloat("_minValueY");
  setProp("_normValueY", (getPropFloat("_valueY") - getPropFloat("_minValueY")) / valueWidthY);
}


///////////////////////////////////////////////////////////////////////////////


  /// ...







///////////////////////////////////////////////////////////////////////////////





} // namespace gloost






