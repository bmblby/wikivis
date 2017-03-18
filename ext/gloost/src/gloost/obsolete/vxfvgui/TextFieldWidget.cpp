
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
#include <gloost/vxfvgui/TextFieldWidget.h>
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
  \class   VxfvWidget

  \brief   A button widget

  \author  Felix Weiszig
  \date    May 2011
  \remarks ..
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

TextFieldWidget::TextFieldWidget(VxfvGui* gui):
    VxfvWidget(gui),
    _value(""),
    _viewValue(""),
    _editing(false)
{
//  gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

TextFieldWidget::~TextFieldWidget()
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
TextFieldWidget::onInitWidget()
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
TextFieldWidget::onEnterFrame()
{
  gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);
  int titleLength = writer->getLineLength(_title+_value);

  _scale[0] = titleLength + 5;
  _scale[1] = writer->getLineHeight();

  _stopEditingEvent = false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
TextFieldWidget::onExitFrame()
{


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseEnterEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseLeaveEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseDownEvent()
{
  if (_window && _window->_isActive)
  {
    _editing = true;
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseUpEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseDownOutsideEvent()
{
  _editing = false;
//  _value = _viewValue;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseUpOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
TextFieldWidget::onMouseOutside()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks Should return TRUE if the widget handled the key
*/

/*virtual */
bool
TextFieldWidget::onHandleKey(unsigned char key, bool state)
{
//  std::cerr << std::endl << "key: " << (int)key;

  if (_editing)
  {
    if (key == 8 && _value.length())
    {
      _value = _value.substr(0, _value.length()-1);
    }
    else if (key == 13)
    {
      _editing = false;
      _stopEditingEvent = true;
    }
    else if (key >= 32 && key <= 127)
    {
      _value = _value + gloost::toString(key);
    }
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
TextFieldWidget::onDraw()
{
  float borderWidth  = 7;
  float height       = 22;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {
      gloost::FreeTypeWriter* writer = _gui->getScreenWriter(VXFV_VXFVGUI_WRITER_BUTTONS);
      writer->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_LEFT);
      int titleLength = writer->getLineLength(_title);


      glTranslated((int)_position[0],
                   (int)_position[1],
                   (int)_position[2]);

      glDisable(GL_TEXTURE_2D);
      glEnable(GL_COLOR_MATERIAL);

      glPushMatrix();
      {
        glColor4f(1.0f, 1.0f, 1.0f, 0.2);
        glScaled(_scale[0], _scale[1], 1.0);
        gl::drawQuad();
      }
      glPopMatrix();

      glColor4f(1.0f, 1.0f, 1.0f, 1.0);


      std::string cursorString = "";

      if (_editing)
      {
        cursorString = "|";
      }


      writer->beginText();
      {
        glColor4f(0.0,0.0,0.0, 1.0);
        writer->writeLine(0.0,
                          1.0,
                         _title);

        writer->writeLine(titleLength,
                          1.0,
                         _value + cursorString);
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
TextFieldWidget::onUpdateMesh()
{

}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


