
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
#include <gloost/gl/Tbo.h>
#include <gloost/gloostConfig.h>
#include <gloost/glErrorUtil.h>
#include <gloost/BinaryBundle.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

namespace gl
{

/**
  \class Tbo

  \brief  Texture Buffer Object

  \author Felix Weiszig
  \date   April 2012
  \remarks Thanks for this helpfull post: http://www.jotschi.de/?p=424
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

Tbo::Tbo(const std::shared_ptr<BinaryBundle>& data,
         unsigned                 usage):
  MultiGlContextEntity(1),
  _data(data),
  _usage(usage)
{
//  _data->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
  \remarks ...
*/

Tbo::~Tbo()
{
  for (unsigned int i=0; i!=GLOOST_SYSTEM_NUM_RENDER_CONTEXTS; ++i)
  {
    removeFromContext(i);
  }
//  _data->dropReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief initialize the resource within a context
  \remarks ...
*/

/*virtual*/
bool
Tbo::initInContext (unsigned int contextId)
{

//  CheckErrorsGL("Tbo::initInContext, befor");


  // create buffer object
  unsigned bufferHandle;

  glGenBuffers(1, &bufferHandle);
  glBindBuffer(GL_TEXTURE_BUFFER, bufferHandle);
  glBufferData(GL_TEXTURE_BUFFER, _data->getSize(), _data->data(), _usage);

  glBindBuffer(GL_TEXTURE_BUFFER, 0);

  setResourceHandleForContext(bufferHandle,
                              contextId,
                              0);


//  CheckErrorsGL("Tbo::initInContext, after");

  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Removes a resource from a context
  \remarks ...
*/

/*virtual*/
void
Tbo::removeFromContext(unsigned int contextId)
{
  if (handleInContextExists(contextId, 0))
  {
    glDeleteBuffers(1, &getResourceHandleForContext(contextId, 0));
    setResourceHandleForContext(0, contextId);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   binds the buffer object into state
  \remarks ...
*/

/*virtual*/
void
Tbo::bind(unsigned int contextId)
{
  if (!handleInContextExists(contextId, 0))
  {
    initInContext(contextId);
  }
  glBindBuffer(GL_TEXTURE_BUFFER, getResourceHandleForContext(contextId, 0));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief unbinds buffer object from state
  \remarks ...
*/

/*static*/
void
Tbo::unbind()
{
  glBindBuffer(GL_TEXTURE_BUFFER, 0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief return the buffers opengl handle for a context
  \remarks ...
*/

/*static*/
unsigned
Tbo::getHandle(unsigned contextId)
{
  if (!handleInContextExists(contextId, 0))
  {
    initInContext(contextId);
  }
  return getResourceHandleForContext(contextId, 0);
}

////////////////////////////////////////////////////////////////////////////////



} // namespace gl
} // namespace gloost
