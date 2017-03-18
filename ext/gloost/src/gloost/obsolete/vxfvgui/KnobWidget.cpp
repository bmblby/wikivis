
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
#include <gloost/vxfvgui/KnobWidget.h>
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

KnobWidget::KnobWidget(VxfvGui* gui,
                       float minValue,
                       float maxValue,
                       float defaultValue,
                       float resolution):
    VxfvWidget(gui),
    _texcoords(),
    _drag(0),
    _value(0.0f),
    _defaultValue(defaultValue),
    _minValue(minValue),
    _maxValue(maxValue),
    _resolution(resolution),
    _normValue( 0.0 )
{
	_texcoords = gloost::vec4(10.0f, 260.0f,  39.0f, 289.0f);

	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);


	_scale = gloost::Vector3(_texcoords.b-_texcoords.r,
                           _texcoords.a-_texcoords.g,
                           1);

  onValueChange();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

KnobWidget::~KnobWidget()
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
KnobWidget::onInitWidget()
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
KnobWidget::onEnterFrame()
{

  // change value while dragging
  if (_drag)
  {
    gloost::Vector3 mouseSpeed = _gui->getMouse()->getSpeed();

    bool mainAxis = mouseSpeed.getMainAxis();
    mouseSpeed[!mainAxis] = 0;

    mouseSpeed[0] *= 0.5;
    mouseSpeed[1] *= 0.05;

    _value += (mouseSpeed[0] + mouseSpeed[1]) * _resolution;

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
KnobWidget::onValueChange()
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
KnobWidget::onExitFrame()
{




}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
KnobWidget::onMouseEnterEvent()
{

  if (_window && _window->_isActive)
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
KnobWidget::onMouseLeaveEvent()
{
  if (_window && !_drag)
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
KnobWidget::onMouseDownEvent()
{
  if (_window && _window->_isActive)
  {
    _drag = true;
  }

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
KnobWidget::onMouseUpEvent()
{
  _drag = false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
KnobWidget::onMouseDownOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
KnobWidget::onMouseUpOutsideEvent()
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
KnobWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
KnobWidget::onMouseOutside()
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
KnobWidget::onDraw()
{

  float textToWidgetDist = 4.0f;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {

      glColor4d(1.0,1.0,1.0,1.0);

      glTranslated((int) (_position[0]+_scale[0]*0.5),
                   (int) (_position[1]+_scale[1]*0.5),
                   (int) (_position[2]));

      glPushMatrix();
      {
        glRotated(140 - _normValue*280, 0.0, 0.0, 1.0);

        _vbo->bind();
        _vbo->draw();
        _vbo->disable();
      }
      glPopMatrix();


      // value text
      gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);
      writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_LEFT);

      writer->beginText();
      {
        glColor4f(0.0,0.0,0.0, 1.0);

        writer->writeLine((int) (_scale[0]*0.5 + textToWidgetDist),
                          (int) (writer->getLineHeight()*-0.5),
                           gloost::toString(_value));
      }
      writer->endText();


      gloost::Point3 scaleWritePos(textToWidgetDist,
                                   (int)  - (_scale[0]  ),
                                   0);


      // title text
      writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_CENTER);

      writer->beginText();
      {
        glColor4f(0.0,0.0,0.0, 1.0);
        writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_CENTER);
        writer->writeLine( scaleWritePos[0],
                           scaleWritePos[1],
                           _title);

        writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_LEFT);
      }
      writer->endText();

      gloost::TextureManager::getInstance()->getTextureWithoutRefcount(_gui->_atlasTextureId)->bind();
      glColor4f(1.0,1.0,1.0, 1.0);

    }
    glPopMatrix();


	}
  glPopAttrib();

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
KnobWidget::onUpdateMesh()
{

  float atlasSize    = 512;


  _mesh->clear();

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();


  gloost::Vector3 offsetToWindow(0.0,0.0,0.0);

  if (_window)
  {
    offsetToWindow[0] = _window->_position[0];
    offsetToWindow[1] = _window->_position[1] + _window->_scale[1] - _scale[1];
  }


  gloost::vec2 texMin(_texcoords.r+(bool)_mouseWithin*_scale[0], _texcoords.g);
  gloost::vec2 texMax(_texcoords.b+(bool)_mouseWithin*_scale[1], _texcoords.a);


  // build mesh
	pushQuadCenter( 0.0,
                  0.0,
                  _position[2],
                  _scale[0], _scale[1], 1.0,
                  (texMin.u)/atlasSize, (texMax.v)/atlasSize,
                  (_scale[0])/atlasSize, (-_scale[1])/atlasSize,
                  _mesh);
}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


