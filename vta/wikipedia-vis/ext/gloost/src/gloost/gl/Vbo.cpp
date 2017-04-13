
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
#include <gloost/gl/Vbo.h>
#include <gloost/gloostConfig.h>
#include <gloost/InterleavedAttributes.h>
#include <gloost/glErrorUtil.h>


// cpp includes
#include <string>
#include <iostream>


namespace gloost
{

namespace gl
{

/**
  \class Vbo

  \brief  Vertex Buffer Object with an Vertex Array Object stroring its layout

  \author Felix Weiszig, Henning Gruendl
  \date   November 2011
  \remarks
*/

#ifdef DEBUG_COUNT_INSTANCES
/*static*/ std::atomic<int> Vbo::_instanceCounter;
#endif

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class constructor
  \remarks ...
*/

Vbo::Vbo(const std::shared_ptr<const gloost::InterleavedAttributes>& vertexAttribs, unsigned usageMode):
  MultiGlContextEntity(2u),
  _vertexAttributes(vertexAttribs),
  _usageMode(usageMode)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class factory
  \remarks ...
*/
Vbo::shared_ptr
Vbo::create(const std::shared_ptr<const gloost::InterleavedAttributes>& vertexAttribs, unsigned usageMode)
{
  return shared_ptr(new Vbo(vertexAttribs, usageMode));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class destructor
  \remarks ...
*/

Vbo::~Vbo()
{  // delete both resources from all contextes
  for (unsigned i=0; i!=GLOOST_SYSTEM_NUM_RENDER_CONTEXTS; ++i)
  {
    removeFromContext(i);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief initialize the resource within a context
  \remarks ...
*/

/*virtual*/
bool
Vbo::initInContext (unsigned contextId)
{
  // generate and bind a vertex array object
  unsigned vaoHandle = 0;
  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);

  setResourceHandleForContext(vaoHandle, contextId, GLOOST_VBO_RESOURCE_VAO);

  // generate and bind vertex buffer object
  unsigned vboHandle = 0;
  glGenBuffers(1, &vboHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);

  setResourceHandleForContext(vboHandle, contextId, GLOOST_VBO_RESOURCE_VBO);

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(float) * _vertexAttributes->getVector().size(),
               &_vertexAttributes->getVector().front(),
               _usageMode);

  // define layout for interleaved attribute data
  const auto& attribInfo = _vertexAttributes->getLayout();

  // determine package stride for one interleaved array
  unsigned packageStride = _vertexAttributes->getPackageStride();
  unsigned currentStride = 0;
  for (unsigned i=0; i!=attribInfo.size(); ++i)
  {
    // enable a VertexAttributeArray
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i,
                          attribInfo[i]._numElements, // number of components
                          GL_FLOAT,
                          GL_FALSE,
                          packageStride,
                          BUFFER_OFFSET(currentStride));

    currentStride += attribInfo[i]._stride;
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Removes a resource from a context
  \remarks ...
*/

/*virtual*/
void
Vbo::removeFromContext(unsigned contextId)
{
  if (handleInContextExists(contextId, GLOOST_VBO_RESOURCE_VBO))
  {
    glDeleteBuffers(1, &getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_VBO));
    setResourceHandleForContext(0, contextId, GLOOST_VBO_RESOURCE_VBO);
  }

  if (handleInContextExists(contextId, GLOOST_VBO_RESOURCE_VAO))
  {
    glDeleteVertexArrays(1, &getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_VAO));
    setResourceHandleForContext(0, contextId, GLOOST_VBO_RESOURCE_VAO);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   binds the drawable into state by specify its components
  \remarks ...
*/

/*virtual*/
void
Vbo::bind(unsigned contextId)
{
  if (!handleInContextExists(contextId, GLOOST_VBO_RESOURCE_VAO))
  {
    initInContext(contextId);
  }
  glBindVertexArray(getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_VAO));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes this drawable from state
  \remarks ...
*/

/*virtual*/
void
Vbo::unbind(unsigned)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0u);
  glBindVertexArray(0u);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   draws the VBO via glDrawArrays
  \remarks Vbo::bind has to be called befor Vbo::draw
*/

/*virtual*/
void
Vbo::draw(unsigned drawMode)
{
  glDrawArrays(drawMode, 0, _vertexAttributes->getNumPackages());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   draws <i>count</i> vertices via glDrawArrays starting from startIndex
  \remarks Vbo::bind has to be called befor Vbo::draw
  \remarks startIndex and count referres to vertex packages within the InterleavedAttributes
*/

/*virtual*/
void
Vbo::draw(unsigned drawMode, unsigned startIndex, unsigned count)
{
  glDrawArrays(drawMode, startIndex, count);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the VertexAttribContainer used by this Vbo
  \remarks
*/

const InterleavedAttributes::const_shared_ptr&
Vbo::getVertexAttributes() const
{
  return _vertexAttributes;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the InterleavedAttributes as data for this Vbo instance
  \remarks removeFromContext has to be called in order to see server side change
*/

void
Vbo::setVertexAttributes(const InterleavedAttributes::shared_ptr& attributes)
{
  _vertexAttributes = attributes;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the GL handle of the VAO
  \remarks This methode will call initInContext if not allready done
*/

unsigned
Vbo::getVaoHandle(unsigned contextId)
{
  if (!handleInContextExists(contextId, GLOOST_VBO_RESOURCE_VAO))
  {
    initInContext(contextId);
  }
  return getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_VAO);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the GL handle of the VBO
  \remarks This methode will call initInContext if not allready done
*/

unsigned
Vbo::getVboHandle(unsigned contextId)
{
  if (!handleInContextExists(contextId, GLOOST_VBO_RESOURCE_VBO))
  {
    initInContext(contextId);
  }
  return getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_VBO);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gl
} // namespace gloost
