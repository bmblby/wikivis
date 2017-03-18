
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



// vxfv system includes
#include <gloost/vxfvgui/SliderWidget.h>
#include <gloost/vxfvgui/WindowWidget.h>
#include <gloost/vxfvgui/VxfvGui.h>

// gloost includes
#include <gloost/gloostConfig.h>
#include <gloost/gl/gloostRenderGoodies.h>
#include <gloost/Mesh.h>
#include <gloost/obsolete/Vbo.h>
#include <gloost/human_input/MouseState.h>
#include <gloost/FreeTypeWriter.h>
#include <gloost/TextureManager.h>
#include <gloost/TileSet.h>

// cpp includes
#include <string>
#include <iostream>
#include <GL/glew.h>



namespace gloost
{

/**
  \class   VxfvWidget

  \brief   Base class for all Widgets used by the gui

  \author  Felix Weiszig
  \date    May 2011
  \remarks ..
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

SliderWidget::SliderWidget(VxfvGui* gui,
                           float minValue,
                           float maxValue,
                           float defaultValue,
                           float resolution,
                           int   length):
    VxfvWidget(gui),
    _texcoords(),
    _knobTexcoords(),
    _drag(0),
    _value(defaultValue),
    _defaultValue(defaultValue),
    _minValue(minValue),
    _maxValue(maxValue),
    _resolution(resolution),
    _normValue( 0.0 ),
    _showText(true),
    _snapValues(),
    _draggingStartPosX(0),
    _draggingStartValue(0),
    _sliderKnobMesh(gloost::Mesh::create()),
    _sliderKnobVbo(new gloost::obsolete::Vbo(_sliderKnobMesh))
{
	_texcoords     = gloost::vec4(10.0f, 195.0f,  46.0f, 213.0f);
  _knobTexcoords = gloost::vec4(11.0f, 231.0f,  28.0f, 247.0f);

	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);

	_sliderKnobMesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_sliderKnobMesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);

	_sliderKnobVbo->takeReference();

	_scale = gloost::Vector3(length,
                           _texcoords.a-_texcoords.g,
                           1);


  onValueChange();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

SliderWidget::~SliderWidget()
{
	_sliderKnobVbo->dropReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::onInitWidget()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::onEnterFrame()
{
  // change value while dragging
  if (_drag)
  {
    _resolution  = (_maxValue - _minValue)/ (_scale[0]-17.0);
    float change = (_gui->getMouse()->getLoc()[0] - _draggingStartPosX)*_resolution;
    _value       = _draggingStartValue + change;

    // snap
    for (unsigned int i=0; i!=_snapValues.size(); ++i)
    {
      if ( abs(_value - _snapValues[i]) < 2.0*_resolution  )
      {
        _value = _snapValues[i];
      }
    }
    onValueChange();
  }

  if ( _mouseWithin && _gui->getMouse()->getButtonDownEvent(GLOOST_MOUSESTATE_BUTTON3) )
  {
    _value = _defaultValue;
    onValueChange();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::onValueChange()
{
  _value = math::clamp(_value, _minValue, _maxValue);
  _normValue = (_value-_minValue) / (_maxValue - _minValue);
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::onExitFrame()
{




}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseEnterEvent()
{
  if (_window)
  {
    onUpdateMesh();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseLeaveEvent()
{
  if (_window)
  {
    _mouseWithin = false;
    onUpdateMesh();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseDownEvent()
{
  if (_window && _window->_isActive)
  {
    float posWithinSlider = _gui->getMouse()->getLoc()[0] - _window->_position[0] - _position[0];
    float relativePos = posWithinSlider/(_scale[0]-9.0);
    _value = _minValue + relativePos*(_maxValue-_minValue);
    onValueChange();

    _drag = true;
    _draggingStartPosX  = _gui->getMouse()->getLastMouseDownLoc()[0];
    _draggingStartValue = _value;
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseUpEvent()
{
  if (_drag)
  {
    _drag = false;
    onUpdateMesh();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseDownOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseUpOutsideEvent()
{
  if (_drag)
  {
    _drag = false;
    onUpdateMesh();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
SliderWidget::onMouseOutside()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::onDraw()
{

  float textToWidgetDist = 3.0f;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {

      glColor4d(1.0,1.0,0.95,1.0);


      glTranslated((int) (_position[0]),
                   (int) (_position[1]),
                   (int) (_position[2]));

      glPushMatrix();
      {
        _vbo->bind();
        _vbo->draw();
        _vbo->disable();

        glTranslated((int) (_normValue*(_scale[0]-(_knobTexcoords.b-_knobTexcoords.r)) ),
                     0.0,
                     (int) (_position[2]));

        _sliderKnobVbo->bind();
        _sliderKnobVbo->draw();
        _sliderKnobVbo->disable();
      }
      glPopMatrix();


      if (_showText)
      {

        // value text
        gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);

        writer->beginText();
        {

          // title text
          writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_LEFT);

          glColor4f(1.0,1.0,1.0, 0.4);
          writer->writeLine( 3 + 1,
                             _scale[1] + textToWidgetDist - 1,
                             _title);
          glColor4f(0.0,0.0,0.0, 1.0);
          writer->writeLine( 3,
                             _scale[1] + textToWidgetDist,
                             _title);


          writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_RIGHT);

          glColor4f(1.0,1.0,1.0, 0.4);
          writer->writeLine((int) (_scale[0]) + 1,
                            (int) _scale[1] + textToWidgetDist - 1,
                            gloost::floatToString(_value, 4));
          glColor4f(0.0,0.0,0.0, 1.0);
          writer->writeLine((int) (_scale[0]),
                            (int) _scale[1] + textToWidgetDist,
                            gloost::floatToString(_value, 4));

        }
        writer->endText();

        gloost::TextureManager::getInstance()->getTextureWithoutRefcount(_gui->_atlasTextureId)->bind();
        glColor4f(1.0,1.0,1.0, 1.0);

      }

    }
    glPopMatrix();


	}
  glPopAttrib();

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a snap value
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::addSnapValue(float value)
{
  _snapValues.push_back(value);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
SliderWidget::onUpdateMesh()
{

  float borderWidth  = 7;
  float height       = 22;
  float atlasSize    = 512;





  _mesh->clear();

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();


  gloost::vec2 texMin(_texcoords.r, _texcoords.g);
  gloost::vec2 texMax(_texcoords.b, _texcoords.a);


  // build mesh


  // left corner
	pushQuad( 0.0,
            0.0,
            _position[2],
            borderWidth, height, 1.0,
            (texMin.u)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-height)/atlasSize,
            _mesh);


  // right corner
	pushQuad( _scale[0] - borderWidth,
            0,
            _position[2],
            borderWidth, height, 1.0,
            (texMax.u-borderWidth)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-height)/atlasSize,
            _mesh);



  // between
	pushQuad( borderWidth,
            0,
            _position[2],
            _scale[0] - borderWidth*2, height, 1.0,
            (texMin.u+borderWidth)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-height)/atlasSize,
            _mesh);



  // geometry for the knob
  _sliderKnobMesh->clear();

  _sliderKnobVbo->dropReference();
  _sliderKnobVbo = new gloost::obsolete::Vbo(_sliderKnobMesh);
  _sliderKnobVbo->takeReference();


  gloost::vec2 knobTexMin(_knobTexcoords.r+ (_mouseWithin||_drag)*19, _knobTexcoords.g);
  gloost::vec2 knobTexMax(_knobTexcoords.b+ (_mouseWithin||_drag)*19, _knobTexcoords.a);


  // build mesh
	pushQuad( 0.0,
            0.0,
            _position[2]+1,
            _knobTexcoords.b-_knobTexcoords.r, 18, 1.0,
            (knobTexMin.u)/atlasSize, (knobTexMax.v)/atlasSize,
            (_knobTexcoords.b-_knobTexcoords.r)/atlasSize, (-18.0)/atlasSize,
            _sliderKnobMesh);



}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


