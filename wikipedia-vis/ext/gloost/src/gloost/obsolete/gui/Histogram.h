
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


#ifndef GLOOST_HISTOGRAM_H
#define GLOOST_HISTOGRAM_H



// gloost includes
#include <gloost/TextureText.h>
#include <gloost/TextLog.h>
#include <gloost/human_input/MouseState.h>
#include <gloost/Vector2.h>


#include <gloost/gui/Widget.h>
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>


/// some defines
#define GLOOST_HISTOGRAM_QUEUE_SIZE          1000
#define GLOOST_HISTOGRAM_DEFAULT_RANGE       250
#define GLOOST_HISTOGRAM_DEFAULT_DRAW_RANGE  50



namespace gloost
{


  /// (Gui widget) XY-Histogram to show multiple values as graphs

class Histogram: public Widget
{
	public:

    /// class constructor
    Histogram( std::string name,
               std::string title,
               gloost::Vector2 position,
               gloost::Vector2 size = gloost::Vector2(300.0f, 120.0f),
               float minValueY = 0.0f,
               float maxvalueY = 1.0f);

    /// class destructor
	  ~Histogram();


    /// loads all necessary resources for this node
    void loadResources();


	  /// render the screen
    void onDraw();


	  /// reset scalation on mouseDown
    void onMouseDown();


    /// histogram-specific methods
    void addToGraph(const float value);


    //sets the default color for the graph
    void setGraphRefColor(const gloost::vec4 col = gloost::vec4(0.9, 0.9, 0.1, 1.0));

    ///sets the center point for interpolation
    ///at this value, the reference color will be used
    void setGraphColorRefValue(const float refValue);

    void setGraphColorRefPercentage(const float refPercentage = 0.5f);

    void setGraphUpperColor(const gloost::vec4 col = gloost::vec4(1.0, 0.1, 0.1, 1.0));

    void setGraphLowerColor(const gloost::vec4 col = gloost::vec4(0.0, 0.4, 0.0, 1.0));

    //sets the color according to the value range and set colors
    //utilizes glColor4f()
    void setRightColor(float val, float alpha = 1.0f);

    void resetTitleColor();

    void resetGraphSize();


    //
    void setQueue(std::vector<float>* values);

    //Sets the size of the ring queue
    void setRange(const unsigned int range);

    //sets the number of elements drawn
    void setDrawRange(const unsigned int range);

    void setQuadSize(const float quadSize);



    //toggles smoothing of values
    void toggleMeanUse();

    void setMeanSize(const unsigned int size = 3);

    /// indexing
    float& operator[](unsigned int);
    const float& operator[](unsigned int) const;


	private:

    /// a simple ring queue used by the Histogram Widget
    struct ringQueue
    {
      std::vector<float>* values ;
      int entryPoint;
      unsigned int range;
    };

	  gloost::TextureText* _textMaker;

	  ringQueue* _graph;


	  bool  _toggleUseMean;
	  unsigned int   _meanSize;


	  bool _toggleUsePercentageColoring;


    gloost::vec4 _color;
    gloost::vec4 _upperColor;
    gloost::vec4 _lowerColor;

    unsigned int _drawRange;

    float _quadWidth;

    float _colorRefValue;
    float _colorRefPercentage;

};
} // namespace gloost


#endif // GLOOST_HISTOGRAM_H








