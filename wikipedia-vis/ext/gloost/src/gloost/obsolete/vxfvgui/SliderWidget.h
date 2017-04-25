
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

#ifndef H_VXFV_SLIDERWIDGET
#define H_VXFV_SLIDERWIDGET



/// vxfv includes
#include <gloost/vxfvgui/VxfvWidget.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <list>




namespace gloost
{

  class MouseState;


  //  Base class for all Widgets used by the gui

class SliderWidget: public VxfvWidget
{
	public:

    // class constructor
    SliderWidget(VxfvGui* gui,
                 float minValue     = 0.0,
                 float maxValue     = 1.0,
                 float defaultValue = 0.0,
                 float resolution   = 0.01,
                 int   length       = 60 );

    // class destructor
	  virtual ~SliderWidget();


    virtual void onInitWidget();
    virtual void onUpdateMesh();

    virtual void onEnterFrame();
    virtual void onExitFrame();


    // HANDLER FOR MOUSE STATES AND EVENTS
    virtual void onMouseEnterEvent();
    virtual void onMouseLeaveEvent();
    virtual void onMouseDownEvent();
    virtual void onMouseUpEvent();
    virtual void onMouseDownOutsideEvent();
    virtual void onMouseUpOutsideEvent();
    virtual void onMouseWithin();
    virtual void onMouseOutside();



    virtual void onDraw();

    virtual void onValueChange();


    // adds a snap value
    void addSnapValue(float value);


    gloost::vec4 _texcoords;
    gloost::vec4 _knobTexcoords;



    bool  _drag;

    float _value;
    float _defaultValue;

	  float _minValue;
	  float _maxValue;
	  float _resolution;

    float _normValue;

    bool _showText;

	protected:

    // container of values, the slider will snapp to while draging
    std::vector<float> _snapValues;
    float              _draggingStartPosX;
    float              _draggingStartValue;

    std::shared_ptr<gloost::Mesh> _sliderKnobMesh;
    gloost::obsolete::Vbo*       _sliderKnobVbo;


};


} // namespace gloost


#endif // H_VXFV_SLIDERWIDGET


