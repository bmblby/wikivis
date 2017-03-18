
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
#include <gloost/vxfvgui/ListWidget.h>
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
#include <algorithm>



namespace gloost
{

/**
  \class   ListWidget

  \brief   A list widget

  \author  Felix Weiszig
  \date    May 2011
  \remarks ..
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

ListWidget::ListWidget(VxfvGui* gui):
    VxfvWidget(gui),
    _texcoords(),
    _value(0),
    _items()
{

	_texcoords = gloost::vec4(10.0, 155.0,  46.0, 177.0);

	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);


	_scale = gloost::Vector3(20, _texcoords.a-_texcoords.g, 1.0 );
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

ListWidget::~ListWidget()
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
ListWidget::onInitWidget()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a item to the list
  \param   ...
  \remarks ...
*/

/*virtual*/
unsigned
ListWidget::addItem(const std::string& itemString)
{
	_items.push_back(itemString);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ListWidget::onEnterFrame()
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
ListWidget::onExitFrame()
{


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseEnterEvent()
{
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseLeaveEvent()
{
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseDownEvent()
{
  if (_window && _window->_isActive)
  {
    _value = true;
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
ListWidget::onMouseUpEvent()
{
  _value = false;
  onUpdateMesh();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseDownOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseUpOutsideEvent()
{
  _value = false;
  onUpdateMesh();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
ListWidget::onMouseOutside()
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
ListWidget::onDraw()
{


  float borderWidth  = 5;
  float height       = 22;

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


      gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);

      writer->beginText();
      {
        glColor4f(0.0,0.0,0.0, 1.0);

        for (unsigned int i=0; i!=_items.size(); ++i)
        {
          writer->writeLine((int)borderWidth,
                            i*(writer->getLineHeight()+2),
                           _items[i]);
        }
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
ListWidget::onUpdateMesh()
{

  float borderWidth  = 7;
  float height       = 22;
  float atlasSize    = 512;


  gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);

  if (_scale[0] < writer->getLineLength(_title))
  {
    _scale[0] = writer->getLineLength(_title) + 2*borderWidth;
  }

  _mesh->clear();

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();


  gloost::vec2 texMin(_texcoords.r+_value*36, _texcoords.g);
  gloost::vec2 texMax(_texcoords.b+_value*36, _texcoords.a);


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
	pushQuad( _scale[0] - borderWidth*1,
            _scale[1]-height,
            _position[2],
            borderWidth, height, 1.0,
            (texMax.u-borderWidth)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-height)/atlasSize,
            _mesh);



  // between
	pushQuad( borderWidth,
            _scale[1] - height,
            _position[2],
            _scale[0] - borderWidth*2, height, 1.0,
            (texMin.u+borderWidth)/atlasSize, (texMax.v)/atlasSize,
            (borderWidth)/atlasSize, (-height)/atlasSize,
            _mesh);
}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


