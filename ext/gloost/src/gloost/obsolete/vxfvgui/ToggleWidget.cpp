
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
#include <gloost/vxfvgui/ToggleWidget.h>
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

ToggleWidget::ToggleWidget(VxfvGui* gui, bool defaultValue):
    VxfvWidget(gui),
    _texcoords(),
    _value(defaultValue),
    _defaultValue(defaultValue)
{

	_texcoords = gloost::vec4(10.0, 120.0,  28.0, 138.0);

	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);



}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

ToggleWidget::~ToggleWidget()
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
ToggleWidget::onInitWidget()
{
//	std::cerr << std::endl << "onInitWidget: " << this;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ToggleWidget::onEnterFrame()
{
  if ( _mouseWithin && _gui->getMouse()->getButtonDownEvent(GLOOST_MOUSESTATE_BUTTON3) )
  {
    _value = _defaultValue;
//    onValueChange();
    onUpdateMesh();
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
ToggleWidget::onExitFrame()
{

//  if (gloost::chance(0.1))
//  {
//    _value = !_value;
//    onUpdateMesh();
//  }


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ToggleWidget::onMouseEnterEvent()
{

  if (_gui->getMouse()->getButtonState(GLOOST_MOUSESTATE_BUTTON3))
  {
    _value = !_value;
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
ToggleWidget::onMouseLeaveEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ToggleWidget::onMouseDownEvent()
{
  if (_window && _window->_isActive)
  {
    _value = !_value;
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
ToggleWidget::onMouseUpEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ToggleWidget::onMouseDownOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ToggleWidget::onMouseUpOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ToggleWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ToggleWidget::onMouseOutside()
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
ToggleWidget::onDraw()
{

  float textToToggleDist = 2.0f;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {
      glTranslated(_position[0], _position[1], _position[2]);

      glColor4f(0.95, 1.0, 0.95, 1.0);

      _vbo->bind();
      _vbo->draw();
      _vbo->disable();

      int toogleWidth  = _texcoords.b-_texcoords.r;
      int toogleHeight = _texcoords.a-_texcoords.g;


      gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);
      writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_LEFT);
      int scaleX = writer->getLineLength(_title);
      _scale = gloost::Vector3(toogleWidth+scaleX+textToToggleDist, toogleHeight,1);

      writer->beginText();
      {
        glColor4f(1.0,1.0,1.0, 0.4);
        writer->writeLine((int) (toogleWidth + textToToggleDist)+1,
                          (int) (toogleHeight/2 - (writer->getLineHeight())/2 + 1)-1,
                         _title);
        glColor4f(0.0,0.0,0.0, 1.0);
        writer->writeLine((int) (toogleWidth + textToToggleDist),
                          (int) (toogleHeight/2 - (writer->getLineHeight())/2 + 1),
                         _title);
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
ToggleWidget::onUpdateMesh()
{
  float borderHeight = 22;
  float atlasSize    = 512;

  _mesh->clear();

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();


  gloost::vec2 texMin(_texcoords.r+_value*18, _texcoords.g);
  gloost::vec2 texMax(_texcoords.b+_value*18, _texcoords.a);


  // build mesh
	pushQuad( 0.0,
            0.0,
            _position[2],
            _texcoords.b-_texcoords.r, _texcoords.a-_texcoords.g, 1.0,
            (texMin.u)/atlasSize, (texMax.v)/atlasSize,
            (_texcoords.b-_texcoords.r)/atlasSize, (-(_texcoords.a-_texcoords.g))/atlasSize,
            _mesh);
}


////////////////////////////////////////////////////////////////////////////////




} // namespace vxfv


