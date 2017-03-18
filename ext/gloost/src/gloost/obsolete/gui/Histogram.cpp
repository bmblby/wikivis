
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

/*

  authors: 2009 by Felix Weiﬂig and Konstantin Silin

*/



/// ggui system includes
#include <gloost/gui/Gui.h>
#include <gloost/gui/Histogram.h>


// gloost includes
#include <gloost/gl/gloostRenderGoodies.h>
#include <gloost/gloostMath.h>



// cpp includes
#include <string>
#include <iostream>
#include <cstring>

namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


/// class constructor

Histogram::Histogram(std::string name,
                     std::string title,
                     gloost::Vector2 position,
                     gloost::Vector2 size,
                     float minValueY,
                     float maxvalueY):
    Widget("gloost_Histogram", name)
    ,_graph()
{


  setProp("_posX",   position[0]);
  setProp("_posY",   position[1]);
  setProp("_width",  size[0]);
  setProp("_height", size[1]);


  // properties
  setProp("_title",        title);

  setProp("_minValueY",     minValueY);
  setProp("_maxValueY",     maxvalueY);

  setProp("_refMinValueY",     minValueY);
  setProp("_refMaxValueY",     maxvalueY);

  setProp("_resizeGraph",     false);  /// shall the histogram adapt to values greater than _maxValueY

  setProp("_titleRed", 1.0f);
  setProp("_titleGreen", 1.0f);
  setProp("_titleBlue", 1.0f);


  setProp("_grid1_subdivX",     20); // subdivision of the finest grid
  setProp("_grid1_subdivY",     20); // subdivision of the finest grid
  setProp("_grid2_subdivX",     4); // subdivision of the medium grid
  setProp("_grid2_subdivY",     4); // subdivision of the medium grid
  setProp("_grid3_subdivX",     2); // subdivision of the big grid
  setProp("_grid3_subdivY",     2); // subdivision of the big grid

  //graphProps
  _graph = new ringQueue();
  _graph->values = new std::vector<float>(GLOOST_HISTOGRAM_QUEUE_SIZE, 0.0f);

  _graph->entryPoint = 0;
  _graph->range = GLOOST_HISTOGRAM_DEFAULT_RANGE;
  _color = gloost::vec4(0.4, 0.4, 0.9, 1.0);
  _upperColor = gloost::vec4(0.4, 0.4, 0.9, 1.0);
  _lowerColor = gloost::vec4(0.4, 0.4, 0.9, 1.0);


  setGraphUpperColor();

  setGraphLowerColor();

  setGraphRefColor();

  setGraphColorRefPercentage();

  _toggleUseMean = false;

  setMeanSize();


  setProp("_defaultQuadWidth", 1.2f);
  setProp("_drawRangeOffset", 0);

  setDrawRange(GLOOST_HISTOGRAM_DEFAULT_DRAW_RANGE);
}


///////////////////////////////////////////////////////////////////////////////


/// class destructor

Histogram::~Histogram()
{
  delete _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


/// loads all necessary resources for this node

void
Histogram::loadResources()
{
  _textMaker = new gloost::TextureText( _gui->getDataFolderPath() +  "/fonts/gloost_Fixedsys_16_gui.png");
  _textMaker->setCharSpace(-8);
}


///////////////////////////////////////////////////////////////////////////////


/// onDraw

void
Histogram::onDraw()
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
    unsigned int grid3_subX = getPropInt("_grid3_subdivX");
    unsigned int grid3_subY = getPropInt("_grid3_subdivY");


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
    glColor4f(0.26f, 0.33f, 0.2f, 0.2f);

    {
      glPushMatrix();
      {
        glTranslated(posX + width/2,
                     posY + height/2,
                     0.0f);
        glScalef(width , height , 1.0f);

        float ar = width/(float)height;

        gloost::gl::drawWireRaster((unsigned int)(grid1_subX*ar), grid1_subY);


        glLineWidth(2.0);
        gloost::gl::drawWireRaster((unsigned int)(grid3_subX*ar), grid3_subY);
      }
      glPopMatrix();
    }



    //for shifting the value point to the right
    //(i.e. drawing the _graph from left to right)
    float valueShift = 0.0f;

    float widthCoeff = (float)width/_drawRange;
    float range = getPropFloat("_maxValueY") - getPropFloat("_minValueY");
    float heightCoeff = (float)height/range;


    float totalSum = 0.0f;

    int offset = getPropInt("_drawRangeOffset");



    for (unsigned int i = _graph->range-_drawRange+offset; i < _graph->range + offset; ++i)
    {

      float currentValue = (*this)[i];

      float shiftX = valueShift*widthCoeff;
      float shiftY = gloost::math::min((float) height, currentValue*heightCoeff);

      setRightColor(currentValue);

      //adding all values to determine the mean value
      totalSum += currentValue;



      glPushMatrix();
      {
        glTranslated(posX + shiftX,
                     posY + 1 ,
                     0.0f);
        glScalef(getPropFloat("_quadWidth"), shiftY-2.0, 1.0f);
        gloost::gl::drawQuad();
      }
      glPopMatrix();

      ++valueShift;
    }



    //render the mean value as line

    float meanValue = (totalSum/(_drawRange + 1));

    setRightColor(meanValue, 0.5f);

    glPushMatrix();
    {
      glTranslated(posX,
                   posY + gloost::math::min( (float) height, meanValue*heightCoeff),
                   0.0f);
      glScalef(width, 2.0, 1.0f);
      glBegin(GL_LINES);

      glVertex3f(0.0, 0.5, 0.0);
      glVertex3f(1.0, 0.5, 0.0);

      glEnd();
    }
    glPopMatrix();






    /// text rendering
    glColor4f(getPropFloat("_titleRed"), getPropFloat("_titleGreen"), getPropFloat("_titleBlue"), 1.0);

    _textMaker->bindFontTexture();

    /// title
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_LEFT);

    _textMaker->setCharScale(1.0);
    _textMaker->renderTextLine(posX,
                               posY + height + 3,
                               getPropString("_title"));

    //render numeric values white
    glColor4f(1.0,1.0, 1.0, 1.0);

    _textMaker->renderTextLine(posX,
                               posY + 3,
                               getPropFloatAsString("_minValueY"));

    _textMaker->renderTextLine(posX,
                               posY + height/2.0f,
                               gloost::toString((getPropFloat("_maxValueY") + getPropFloat("_minValueY"))/2.0f));

    _textMaker->renderTextLine(posX,
                               posY + height - 20,
                               getPropFloatAsString("_maxValueY"));


     _textMaker->renderTextLine(posX + width - 70,
                               posY + gloost::math::min( (float) height, meanValue*heightCoeff),
                               gloost::toString(meanValue));


    _textMaker->setCharScale(1.3);
    _textMaker->setLineAlignment(GLOOST_TEXTURETEXT_ALIGN_CENTER);
    _textMaker->renderTextLine(posX + width/2,
                               posY +3,
                               gloost::toString(_graph->values->at(math::max(0, _graph->entryPoint - 1))));

  }
  glPopAttrib();

}

///////////////////////////////////////////////////////////////////////////////


/// reset scalation on mouseDown

void
Histogram::onMouseDown()
{
  resetGraphSize();
  resetTitleColor();
}


///////////////////////////////////////////////////////////////////////////////


/// add a value to the queue

void
Histogram::addToGraph(float value)
{
  float val = value;
  unsigned int insertPoint = 0;

  ++_graph->entryPoint;

  if ((int)_graph->entryPoint >= (int)_graph->range)
    _graph->entryPoint = 0;

  if (_graph->entryPoint==0)
    insertPoint = _graph->range-1;
  else
  {
    insertPoint = _graph->entryPoint-1;
  }

  if (_toggleUseMean && insertPoint > _meanSize)
  {

    for (int i = 1; i < (int)_meanSize; ++i)
    {
      val +=  _graph->values->at(insertPoint - i);
    }
    val = val/_meanSize;
  }

  _graph->values->at(insertPoint) = val;


  //if resizing is on, reset max values if necessary
  if (getPropBool("_resizeGraph"))
  {

    if (val > getPropFloat("_maxValueY"))
    {

      setProp("_maxValueY", val);
      setProp("_titleRed", 1.0f);
      setProp("_titleGreen", 0.1f);
      setProp("_titleBlue", 0.1f);

    }
    else if (val < getPropFloat("_minValueY"))
    {
      setProp("_minValueY", val);
      setProp("_titleRed", 0.0f);
      setProp("_titleGreen", 0.7f);
      setProp("_titleBlue", 0.0f);

    }
  }
  else
  {

    if (val > getPropFloat("_maxValueY"))
    {

      setProp("_titleRed", 1.0f);
      setProp("_titleGreen", 0.1f);
      setProp("_titleBlue", 0.1f);

    }
    else if (val < getPropFloat("_minValueY"))
    {
      setProp("_titleRed", 0.0f);
      setProp("_titleGreen", 0.7f);
      setProp("_titleBlue", 0.0f);
    }

  }

}



///////////////////////////////////////////////////////////////////////////////


/// sets the middle color of the graph

void
Histogram::setGraphRefColor(gloost::vec4 col)
{
  _color = col;
}

///////////////////////////////////////////////////////////////////////////////


/// sets the reference percentage, at which the coloring is interpolated
/// above this value, we interpolate between the uooer and the middle color
/// below this value, we interpolate between the uooer and the middle color
void
Histogram::setGraphColorRefPercentage(const float refPercentage)
{

  _colorRefPercentage = refPercentage;
}

///////////////////////////////////////////////////////////////////////////////


/// sets the upper color of the graph

void
Histogram::setGraphUpperColor(gloost::vec4 col)
{
  _upperColor = col;
}

///////////////////////////////////////////////////////////////////////////////


/// sets the lower color of the graph

void
Histogram::setGraphLowerColor(gloost::vec4 col)
{
  _lowerColor = col;
}

///////////////////////////////////////////////////////////////////////////////


/// resets the color of the title to white

void
Histogram::setRightColor(float val, float alpha)
{

  float range = getPropFloat("_maxValueY") - getPropFloat("_minValueY");
  float normalizedShift = val/range;
  float colorPercentage = 0.0;

  gloost::vec4 color;


   if (normalizedShift > _colorRefPercentage)
  {

    colorPercentage = (normalizedShift - _colorRefPercentage)*(1/(1-_colorRefPercentage));
    color = _upperColor;

  }
  else//(normalizedShift <= _colorRefPercentage)
  {

    colorPercentage = (_colorRefPercentage - normalizedShift)*(1/_colorRefPercentage);
    color = _lowerColor;
  }

  glColor4f(color.r*colorPercentage + _color.r*(1.0f-colorPercentage)
            , color.g*colorPercentage + _color.g*(1.0f-colorPercentage)
            , color.b*colorPercentage + _color.b*(1.0f-colorPercentage)
            ,( color.a*colorPercentage + _color.a*(1.0f-colorPercentage))*alpha);

}

///////////////////////////////////////////////////////////////////////////////


/// resets the color of the title to white

void
Histogram::resetTitleColor()
{
  setProp("_titleRed", 1.0f);
  setProp("_titleGreen", 1.0f);
  setProp("_titleBlue", 1.0f);
}


///////////////////////////////////////////////////////////////////////////////


/// resets the graph size to initial values

void
Histogram::resetGraphSize()
{

  setProp("_maxValueY", getPropFloat("_refMaxValueY"));
  setProp("_minValueY", getPropFloat("_refMinValueY"));
}

///////////////////////////////////////////////////////////////////////////////


 /// Sets the size of the ring queue

void
Histogram::setRange(const unsigned int range)
{
  _graph->range = range;

  if((int)range > _graph->entryPoint)
  {
    _graph->entryPoint = range;
  }

}


///////////////////////////////////////////////////////////////////////////////


 /// sets the number of elements drawn

void
Histogram::setDrawRange(const unsigned int range)
{

  _drawRange = range;

  setProp("_quadWidth", getPropFloat("_defaultQuadWidth"));

}


///////////////////////////////////////////////////////////////////////////////

/// toggles value smoothing

void
Histogram::toggleMeanUse()
{
  _toggleUseMean = !_toggleUseMean;
}

///////////////////////////////////////////////////////////////////////////////
/// sets the size of the smoothing kernel
void
Histogram::setMeanSize(unsigned int size)
{
  _meanSize = size;
}


///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////


float&
Histogram::operator[](unsigned int i)
{


  if(i < _graph->range )
  {
    unsigned int index = (_graph->entryPoint + i )% _graph->range;
    return (*_graph->values)[index];
  }


    std::cout << std::endl;
    std::cout << std::endl << "ERROR in Histogram::operator[](unsigned int i):" ;
		std::cout << std::endl << "         index " << i << " is out of range";
		std::cout << std::endl << "         the graph has a range of "  << _graph->range;
		std::cout << std::endl << "         returning the value at entryPoint ("  << _graph->entryPoint << ")";
		std::flush(std::cout);


  return (*_graph->values)[_graph->entryPoint];

}


////////////////////////////////////////////////////////////////////////////////


const float&
Histogram::operator[] (unsigned int i) const
{

  if(i < _graph->range )
  {
    unsigned int index = (_graph->entryPoint + i )% _graph->range;
    return (*_graph->values)[index];
  }


    std::cout << std::endl;
    std::cout << std::endl << "ERROR in Histogram::operator[](unsigned int i):" ;
		std::cout << std::endl << "         index " << i << " is out of range";
		std::cout << std::endl << "         the graph has a range of "  << _graph->range;
		std::cout << std::endl << "         returning the value at entryPoint ("  << _graph->entryPoint << ")";
		std::flush(std::cout);


  return (*_graph->values)[_graph->entryPoint];

}



} // namespace gloost







