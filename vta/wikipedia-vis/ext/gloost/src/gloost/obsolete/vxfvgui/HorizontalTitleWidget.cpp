
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
#include <gloost/vxfvgui/HorizontalTitleWidget.h>
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
  \class   HorizontalTitleWidget

  \brief   A horizontal line with a title to use with the VxfvGui

  \author  Felix Weiszig
  \date    May 2011
  \remarks ..
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

HorizontalTitleWidget::HorizontalTitleWidget(VxfvGui* gui):
    VxfvWidget(gui)
{
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_COLORS);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

HorizontalTitleWidget::~HorizontalTitleWidget()
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
HorizontalTitleWidget::onInitWidget()
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
HorizontalTitleWidget::onEnterFrame()
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
HorizontalTitleWidget::onExitFrame()
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
HorizontalTitleWidget::onDraw()
{


  float borderWidth  = 7;
  float height       = 22;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {
      glTranslated((int)_position[0],
                   (int)_position[1],
                   (int)_position[2]);

      glDisable(GL_TEXTURE_2D);

      glColor4f(0.0,0.0,0.0, 1.0);
      _vbo->bind();
      _vbo->draw();
      _vbo->disable();

//      std::cerr << std::endl << "_position: " << _position;
//      std::cerr << std::endl << "_scale: "    << _scale;
//      std::cerr << std::endl << "lines: "     << _vbo->getMesh()->getLines().size();


      gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_SMALL_VALUES);
      writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_CENTER);

      glEnable(GL_TEXTURE_2D);

      writer->beginText();
      {
        glColor4f(1.0,1.0,1.0, 0.4);
        writer->writeLine((int)(_scale[0]*0.5 +1),
                          (int) (_scale[1] - (writer->getLineHeight()+height)*0.5 + 1),
                         _title);

        glColor4f(0.1,0.1,0.1, 1.0);
        writer->writeLine((int)(_scale[0]*0.5),
                          (int) (_scale[1] - (writer->getLineHeight()+height)*0.5 + 2),
                         _title);
      }
      writer->endText();

//      gloost::TextureManager::getInstance()->getTextureWithoutRefcount(_gui->_atlasTextureId)->bind();
//      glColor4f(1.0,1.0,1.0, 1.0);

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
HorizontalTitleWidget::onUpdateMesh()
{

  float height             = 0;

  gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_SMALL_VALUES);

  int textWidth = writer->getLineLength(_title);

  int linesFrontStart = 0;
  int linesFrontEnd   = (_scale[0] - textWidth) * 0.5;

  int linesBackStart = (_scale[0] + textWidth) * 0.5;
  int linesBackEnd   = _scale[0];

  _mesh->clear();

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();


  // creating two colored lines

  // 1 front
  _mesh->pushInterleavedComponent(gloost::Point3(linesFrontStart,
                                                  height,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(1.0, 1.0, 1.0, 1.0));



  _mesh->pushInterleavedComponent(gloost::Point3(linesFrontEnd,
                                                  height,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(1.0, 1.0, 1.0, 1.0));

  _mesh->getLines().push_back(gloost::Mesh::LineIndices(0,1));

  // 2 front
  _mesh->pushInterleavedComponent(gloost::Point3(linesFrontStart,
                                                  height+1,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(0.0, 0.0, 0.0, 1.0));



  _mesh->pushInterleavedComponent(gloost::Point3(linesFrontEnd,
                                                  height+1,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(0.0, 0.0, 0.0, 1.0));

  _mesh->getLines().push_back(gloost::Mesh::LineIndices(2,3));


  // 1 back
  _mesh->pushInterleavedComponent(gloost::Point3(linesBackStart,
                                                  height,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(1.0, 1.0, 1.0, 1.0));



  _mesh->pushInterleavedComponent(gloost::Point3(linesBackEnd,
                                                  height,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(1.0, 1.0, 1.0, 1.0));

  _mesh->getLines().push_back(gloost::Mesh::LineIndices(4,5));

  // 2 back
  _mesh->pushInterleavedComponent(gloost::Point3(linesBackStart,
                                                  height+1,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(0.0, 0.0, 0.0, 1.0));



  _mesh->pushInterleavedComponent(gloost::Point3(linesBackEnd,
                                                  height+1,
                                                  0.0));

  _mesh->pushInterleavedComponent(vec4(0.0, 0.0, 0.0, 1.0));

  _mesh->getLines().push_back(gloost::Mesh::LineIndices(6,7));
}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


