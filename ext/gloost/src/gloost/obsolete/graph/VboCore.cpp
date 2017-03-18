
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



#include <gloost/graph/Action.h>
#include <gloost/Material.h>

#include <gloost/graph/VboCore.h>



namespace gloost
{
namespace graph
{



////////////////////////////////////////////////////////////////////////////////


 /// class constructor

VboCore::VboCore(obsolete::Vbo* vbo, unsigned int coreEnterMode, unsigned int coreExitMode):
  Core(GLOOST_CORE_VBO),
  _vbo(vbo->takeReference()),
  _coreEnterMode(coreEnterMode),
  _coreExitMode(coreExitMode)
{

}


////////////////////////////////////////////////////////////////////////////////


 /// class destructor

VboCore::~VboCore()
{
	_vbo->dropReference();
}


////////////////////////////////////////////////////////////////////////////////


 ///

/* virtual */
void
VboCore::enterCore (const Action* action)
{

  if (_disabled)
  {
    return;
  }

  unsigned int contextId = action->getContextId();


  switch (_coreEnterMode)
  {
    case GLOOST_VBOCORE_BIND:
      _vbo->bind();
      break;
    case GLOOST_VBOCORE_UNBIND:
      _vbo->disable();
      break;
    case GLOOST_VBOCORE_BIND_AND_DRAW:
      _vbo->bind();
      _vbo->draw();
      break;
    case GLOOST_VBOCORE_DRAW:
      _vbo->draw();
      break;
    case GLOOST_VBOCORE_NOTHING:
      break;
  }

}


////////////////////////////////////////////////////////////////////////////////


 ///

/* virtual */
void
VboCore::exitCore (const Action* action)
{

  unsigned int contextId = action->getContextId();

  switch (_coreExitMode)
  {
    case GLOOST_VBOCORE_BIND:
      _vbo->bind();
      break;
    case GLOOST_VBOCORE_UNBIND:
      _vbo->disable();
      break;
    case GLOOST_VBOCORE_BIND_AND_DRAW:
      _vbo->bind();
      _vbo->draw();
      break;
    case GLOOST_VBOCORE_DRAW:
      _vbo->draw();
      break;
    case GLOOST_VBOCORE_NOTHING:
      break;
  }
}


////////////////////////////////////////////////////////////////////////////////


 /// get the geometry

obsolete::Vbo*
VboCore::getVbo()
{
	return _vbo;
}


////////////////////////////////////////////////////////////////////////////////


 /// set a geometry

void
VboCore::setVbo(obsolete::Vbo* vbo)
{
	_vbo = vbo;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets behavior of the VboCore if enterCore() is called
  \param   mode can be GLOOST_VBOCORE_NOTHING, GLOOST_VBOCORE_BIND, GLOOST_VBOCORE_UNBIND, GLOOST_VBOCORE_BIND_AND_DRAW or GLOOST_VBOCORE_DRAW
  \remarks ...
*/

void
VboCore::setCoreEnterMode(unsigned int mode)
{
	_coreEnterMode = mode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets behavior of the VboCore if enterCore() is called
  \param   mode can be GLOOST_VBOCORE_NOTHING, GLOOST_VBOCORE_BIND, GLOOST_VBOCORE_UNBIND, GLOOST_VBOCORE_BIND_AND_DRAW or GLOOST_VBOCORE_DRAW
  \remarks ...
*/

void
VboCore::setCoreExitMode(unsigned int mode)
{
	_coreExitMode = mode;
}


////////////////////////////////////////////////////////////////////////////////


VboCore*
VboCore::operator= (Core* core)
{
  return static_cast<VboCore*> (core);
}


////////////////////////////////////////////////////////////////////////////////


} // namespace graph
} // namespace gloost

