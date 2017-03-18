
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
#include <gloost/vxfvgui/WindowBorderWidget.h>
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

WindowBorderWidget::WindowBorderWidget(VxfvGui* gui):
    VxfvWidget(gui),
    _texcoords(),
    _dragable(true),
    _windowMinimized(false)
{

	_texcoords = gloost::vec4(10.0, 84.0,  74.0, 106.0);

	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);


	_scale = gloost::Vector3(50.0, _texcoords.a-_texcoords.g, 1.0 );

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

WindowBorderWidget::~WindowBorderWidget()
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
WindowBorderWidget::onInitWidget()
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
WindowBorderWidget::onEnterFrame()
{
  if (_window)
  {
    if (_drag)
    {
      _window->_position = _window->_position + _gui->getMouse()->getSpeed();
    }

//    if (_window->_isActive && _mouseWithin && _gui->getMouse()->getButtonDownEvent(GLOOST_MOUSESTATE_BUTTON3))
//    {
//      _window->_position[1] = -_window->_scale[1] + 22;
//    }
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
WindowBorderWidget::onExitFrame()
{


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowBorderWidget::onMouseEnterEvent()
{

//  if(_window->_minimized)
//  {
//    _window->_scale[1] = 300;
//    _window->_minimized = false;
////    _window->onUpdateMesh();
//  }


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowBorderWidget::onMouseLeaveEvent()
{
//  if(!_window->_minimized)
//  {
//    _window->_scale[1]  = 64;
//    _window->_minimized = true;
////    _window->onUpdateMesh();
//  }

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowBorderWidget::onMouseDownEvent()
{

  // minimize if clickt
  if (_window)
  {
    if (_gui->getMouse()->getLoc()[0] > _window->_position[0]+_window->_scale[0]-18)
    {
      _window->_minimized = true;
      return;
    }
  }


  if (_window && _window->_isActive && _dragable)
  {
    _drag = true;
  }

  onUpdateMesh();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowBorderWidget::onMouseUpEvent()
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
WindowBorderWidget::onMouseDownOutsideEvent()
{
  onUpdateMesh();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowBorderWidget::onMouseUpOutsideEvent()
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
WindowBorderWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowBorderWidget::onMouseOutside()
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
WindowBorderWidget::onDraw()
{
  float borderWidth  = 6;
  float borderHeight = 22;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {
      glTranslated((int)_position[0],
                   (int)_position[1],
                   (int)_position[2]);

      _vbo->bind();
      _vbo->draw();
      _vbo->disable();

      gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_WINDOWBORDER);
      writer->beginText();
      {
        glColor4f(0.0,0.0,0.0, 0.6);

        if (_window->_isActive)
        {
          glColor4f(0.0,0.0,0.0, 0.75);
        }

        writer->writeLine((int)borderWidth,
                          (int) (_scale[1] - (writer->getLineHeight()+borderHeight)/2 + 2),
                         _window->_title);
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
WindowBorderWidget::onUpdateMesh()
{
  float borderWidth  = 23;
  float borderHeight = 22;
  float atlasSize    = 512;

  if (_window)
  {
    _position[0] = 0.0;
    _position[1] = _window->_scale[1] - borderHeight;
    _position[2] = 0.0;

    _scale[0] = _window->_scale[0];
    _scale[1] = borderHeight;
    _scale[2] = 1;
  }

  _mesh->clear();

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();

  gloost::Vector3 offsetToWindow(0.0,0.0,0.0);

  if (_window)
  {
    offsetToWindow[0] = _window->_position[0];
    offsetToWindow[1] = _window->_position[1] + _window->_scale[1] - borderHeight;
  }

  gloost::vec2 texMin(_texcoords.r+_window->_isActive*74, _texcoords.g);
  gloost::vec2 texMax(_texcoords.b+_window->_isActive*74, _texcoords.a);

  // build mesh
  // left corner
	pushQuad( 0.0,
            0.0,
            _position[2],
            borderWidth, borderHeight, 1.0,
            (texMin.u)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-borderHeight)/atlasSize,
            _mesh);


  // right corner
	pushQuad( _window->_scale[0] - borderWidth,
            _scale[1]-borderHeight,
            _position[2],
            borderWidth, borderHeight, 1.0,
            (texMax.u-borderWidth)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-borderHeight)/atlasSize,
            _mesh);



  // between
	pushQuad( borderWidth,
            _scale[1] - borderHeight,
            _position[2],
            _window->_scale[0] - borderWidth*2, borderHeight, 1.0,
            (texMin.u+borderWidth)/atlasSize, (texMax.v)/atlasSize,
            (2.0)/atlasSize, (-borderHeight)/atlasSize,
            _mesh);
}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


