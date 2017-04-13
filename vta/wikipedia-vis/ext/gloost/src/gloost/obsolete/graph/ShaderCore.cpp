
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



// gloost system includes
#include <gloost/graph/ShaderCore.h>
#include <gloost/graph/Action.h>
#include <gloost/gl/UniformSet.h>
#include <gloost/gl/ShaderProgram.h>
#include <gloost/gloostConfig.h>



namespace gloost
{
namespace graph
{




////////////////////////////////////////////////////////////////////////////////

 // Needed to get the shader that was set befor,
 // if a action goes up in the graph

/* static */
std::vector< std::stack<gl::ShaderProgram*> > ShaderCore::_shaderStackVector = std::vector< std::stack<gl::ShaderProgram*> >(GLOOST_SYSTEM_NUM_RENDER_CONTEXTS, std::stack<gl::ShaderProgram*>());

////////////////////////////////////////////////////////////////////////////////


 /// class constructor

ShaderCore::ShaderCore(gl::ShaderProgram* shader):
  Core(GLOOST_CORE_SHADER),
  _shader(shader),
  _uniformSet(0)
{
  _shader->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


 /// class constructor

ShaderCore::ShaderCore(gl::UniformSet* uSet):
  Core(GLOOST_CORE_SHADER),
  _shader(0),
  _uniformSet(uSet)
{
//  _uniformSet->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


 /// class constructor

ShaderCore::ShaderCore(gl::ShaderProgram* shader, gl::UniformSet* uSet):
  Core(GLOOST_CORE_SHADER),
  _shader(shader),
  _uniformSet(uSet)
{
  _shader->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


// class destructor
ShaderCore::~ShaderCore()
{
  if (_shader)
  {
    _shader->dropReference();
  }
}


////////////////////////////////////////////////////////////////////////////////


/* virtual */
void
ShaderCore::enterCore (const Action* action)
{
  if (_disabled)
  {
    return;

  }

  unsigned int currentContextId = action->getContextId();

  /// push this Shader to the stack and set it
  if (_shader)
  {
    _shaderStackVector[currentContextId].push(_shader);
    _shaderStackVector[currentContextId].top()->use(currentContextId);
  }

  /// configure the Shader with the UniformSet
  if (_uniformSet && _shaderStackVector[currentContextId].size())
  {
    _uniformSet->applyToShader(_shaderStackVector[currentContextId].top(),
                               currentContextId);
  }

}


////////////////////////////////////////////////////////////////////////////////


/* virtual */
void
ShaderCore::exitCore(const Action* action)
{
  if (_disabled)
  {
    return;
  }

  /// get context of the action
  unsigned int currentContextId = action->getContextId();

  if (_shader)
  {
    _shaderStackVector[currentContextId].top()->disable();
    _shaderStackVector[currentContextId].pop();
  }

  if (!_shaderStackVector[currentContextId].empty())
  {
    _shaderStackVector[currentContextId].top()->use(currentContextId);
  }
}


////////////////////////////////////////////////////////////////////////////////


void
ShaderCore::setShader (gl::ShaderProgram *shader)
{
  _shader = shader;
}


////////////////////////////////////////////////////////////////////////////////


const gl::ShaderProgram*
ShaderCore::getShader () const
{
  return _shader;
}


////////////////////////////////////////////////////////////////////////////////


void
ShaderCore::setUniformSet(gl::UniformSet* uSet)
{
  _uniformSet = uSet;
}


////////////////////////////////////////////////////////////////////////////////


gl::UniformSet*
ShaderCore::getUniformSet() const
{
  return _uniformSet;
}


////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

} // namespace graph
} // namespace gloost


