
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
#include <gloost/vxfvgui/WindowWidget.h>
#include <gloost/vxfvgui/VxfvGui.h>

// gloost includes
#include <gloost/gloostConfig.h>
#include <gloost/gl/gloostRenderGoodies.h>
#include <gloost/Mesh.h>
#include <gloost/obsolete/Vbo.h>
#include <gloost/human_input/MouseState.h>
#include <gloost/TextureManager.h>

// cpp includes
#include <string>
#include <iostream>



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

WindowWidget::WindowWidget(VxfvGui* gui):
    VxfvWidget(gui),
    _backgroundColor(1.0,1.0,1.0,1.0),
    _texcoords(),
    _isActive(false),
    _minimized(false),
    _widgets(),
    _widgetsByName()
{

	_texcoords = gloost::vec4(10.0, 10.0,  74.0, 74.0);

	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS);
	_mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

WindowWidget::~WindowWidget()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a child widget and returns a global id
  \param   ...
  \remarks ...
*/

unsigned
WindowWidget::addWidget(VxfvWidget* widget, const std::string& name)
{
  if (widget->_window)
  {
    return 0;
  }

  _widgetsByName[name] = widget;

  widget->_window = this;

  widget->onUpdateMesh();

	_widgets.push_back(widget);

	return _gui->registerWidget(widget);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

void
WindowWidget::onHandleMouse(gloost::human_input::MouseState* mouse)
{
  VxfvWidget::onHandleMouse(mouse);

  if (_minimized)
  {
    return;
  }

  WidgetListIterator widgetIt    = _widgets.begin();
  WidgetListIterator widgetEndIt = _widgets.end();

  for (; widgetIt!=widgetEndIt; ++widgetIt)
  {
    (*widgetIt)->onHandleMouse(mouse);
  }

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a child widget
  \param   ...
  \remarks ...
*/

void
WindowWidget::removeWidget(VxfvWidget* widget)
{
  widget->_window = 0;
	_widgets.remove(widget);

	std::map<std::string, VxfvWidget*>::iterator it    = _widgetsByName.begin();
	std::map<std::string, VxfvWidget*>::iterator endit = _widgetsByName.end();
	for (; it != endit; ++it)
	{
	  if ( (*it).second == widget )
	  {
	    _widgetsByName.erase(it);
	    break;
	  }
	}
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a Widget for a name or 0
  \param   ...
  \remarks ...
*/

VxfvWidget*
WindowWidget::getWidget(const std::string& name)
{
	std::map<std::string, VxfvWidget*>::iterator it = _widgetsByName.find(name);
	if (it != _widgetsByName.end())
	{
	  return (*it).second;
	}

  std::cerr << std::endl << "Message from WindowWidget::getWidget(const std::string& name): ";
  std::cerr << std::endl << "             Could not found name: " << name;
  std::cerr << std::endl;
  std::cerr << std::endl;
	return 0;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a reference to the list of widgets of this window
  \param   ...
  \remarks ...
*/

WindowWidget::WidgetList&
WindowWidget::getWidgetList()
{
	return _widgets;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a const reference to the list of widgets of this window
  \param   ...
  \remarks ...
*/

const WindowWidget::WidgetList&
WindowWidget::getWidgetList() const
{
	return _widgets;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*virtual*/
void
WindowWidget::onInitWidget()
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
WindowWidget::onEnterFrame()
{
  WidgetListIterator widgetIt    = _widgets.begin();
  WidgetListIterator widgetEndIt = _widgets.end();

  for (; widgetIt!=widgetEndIt; ++widgetIt)
  {
    (*widgetIt)->onEnterFrame();
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
WindowWidget::onExitFrame()
{
  WidgetListIterator widgetIt    = _widgets.begin();
  WidgetListIterator widgetEndIt = _widgets.end();

  for (; widgetIt!=widgetEndIt; ++widgetIt)
  {
    (*widgetIt)->onExitFrame();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseEnterEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseLeaveEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseDownEvent()
{
  if (_isActive)
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
WindowWidget::onMouseUpEvent()
{
//  if (_isActive)
  {
    _drag = false;
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseDownOutsideEvent()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseUpOutsideEvent()
{
//  if (_isActive)
  {
    _drag = false;
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseWithin()
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual */
void
WindowWidget::onMouseOutside()
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
WindowWidget::onDraw()
{

  if (_minimized)
  {
    return;
  }


	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
    glPushMatrix();
    {
      glTranslated((int)_position[0],
                   (int)_position[1],
                   (int)_position[2]);

      gloost::TextureManager::getInstance()->getTextureWithoutRefcount(_gui->_atlasTextureId)->bind();
      glColor4d(1.0,1.0,1.0,0.95);

//      gloost::Matrix scale = gloost::Matrix::createScaleMatrix(1.0, 0.5, 1.0);
//      gloostMultMatrix(scale.data());

      _vbo->bind();
      _vbo->draw();
      _vbo->disable();

      // draw all widgets
      std::list<VxfvWidget*>::iterator widgetIt    = _widgets.begin();
      std::list<VxfvWidget*>::iterator widgetEndIt = _widgets.end();

      for (; widgetIt!=widgetEndIt; ++widgetIt)
      {
        (*widgetIt)->onDraw();
      }
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
WindowWidget::onUpdateMesh()
{

  _mesh->clear();

  float borderWidth = 6;
  float atlasSize   = 512;

  _vbo->dropReference();
  _vbo = new gloost::obsolete::Vbo(_mesh);
  _vbo->takeReference();

//  gloost::vec2 texMin(_texcoords.r+_isActive*74, _texcoords.g);
//  gloost::vec2 texMax(_texcoords.b+_isActive*74, _texcoords.a);
  gloost::vec2 texMin(_texcoords.r, _texcoords.g);
  gloost::vec2 texMax(_texcoords.b, _texcoords.a);

  // build mesh

  // lower left corner
	pushQuad( 0, 0, 0.0,
            borderWidth, borderWidth, 1.0,
            (texMin.u)/atlasSize, (texMin.v)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // lower right corner
	pushQuad( _scale[0]-borderWidth, 0, 0.0,
            borderWidth, borderWidth, 1.0,
            (texMax.u-borderWidth)/atlasSize, (texMin.v)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // upper left corner
	pushQuad( 0.0, _scale[1]-borderWidth, 0.0,
            borderWidth, borderWidth, 1.0,
            (texMin.u)/atlasSize, (texMax.v-borderWidth)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // upper left corner
	pushQuad( _scale[0]-borderWidth, _scale[1]-borderWidth, 0.0,
            borderWidth, borderWidth, 1.0,
            (texMax.u-borderWidth)/atlasSize, (texMax.v-borderWidth)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);



  // upper border
	pushQuad( borderWidth, _scale[1]-borderWidth, 0.0,
            _scale[0] - borderWidth*2, borderWidth, 1.0,
            (texMin.u+borderWidth)/atlasSize, (texMax.v-borderWidth)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // lower border
	pushQuad( borderWidth, 0.0, 0.0,
            _scale[0] - borderWidth*2, borderWidth, 1.0,
            (texMin.u+borderWidth)/atlasSize, (texMin.v)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // left border
	pushQuad( 0.0, borderWidth, 0.0,
            borderWidth, _scale[1] - borderWidth*2, 1.0,
            (texMin.u)/atlasSize, (texMin.v+borderWidth)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // right border
	pushQuad( _scale[0]-borderWidth, borderWidth, 0.0,
            borderWidth, _scale[1] - borderWidth*2, 1.0,
            (texMax.u-borderWidth)/atlasSize, (texMin.v+borderWidth)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);

  // center
	pushQuad( borderWidth, borderWidth, 0.0,
            _scale[0] - borderWidth*2, _scale[1] - borderWidth*2, 1.0,
            (texMin.u+borderWidth)/atlasSize, (texMin.v+borderWidth)/atlasSize,
            (borderWidth)/atlasSize, (borderWidth)/atlasSize,
            _mesh);



}


////////////////////////////////////////////////////////////////////////////////




} // namespace gloost


