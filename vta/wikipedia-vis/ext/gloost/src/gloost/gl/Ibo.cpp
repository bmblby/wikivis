
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
#include <gloost/gl/Ibo.h>
#include <gloost/gloostConfig.h>
#include <gloost/Mesh.h>
#include <gloost/glErrorUtil.h>



// cpp includes
#include <string>
#include <iostream>
#include <GL/glew.h>



namespace gloost
{
namespace gl
{

/**
  \class Ibo

  \brief  Index Buffer Object to use with a gloost::obsolete::Vbo

  \author Felix Weiszig, Henning Gruendl
  \date   November 2011
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

Ibo::Ibo(const std::shared_ptr<Mesh>& mesh, unsigned drawMode):
  MultiGlContextEntity(1u),
  _mesh(mesh),
  _drawMode(drawMode),
  _suggestedDrawMode(0u),
  _numIdices(0u)
{
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

Ibo::Ibo(const std::shared_ptr<const Mesh>& mesh, unsigned drawMode):
  MultiGlContextEntity(1u),
  _mesh(mesh),
  _drawMode(drawMode),
  _suggestedDrawMode(0u),
  _numIdices(0u)
{
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

Ibo::Ibo(const std::vector<unsigned>& indices, unsigned drawMode):
  MultiGlContextEntity(1u),
  _mesh(nullptr),
  _drawMode(drawMode),
  _suggestedDrawMode(0u),
  _numIdices(0u)
{
  auto mesh = gloost::Mesh::create();
  mesh->getPoints() = indices;
  _mesh = mesh;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class factory
  \remarks ...
*/
Ibo::shared_ptr
Ibo::create(const std::shared_ptr<Mesh>& mesh, unsigned drawMode)
{
  return shared_ptr(new Ibo(mesh, drawMode));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
  \remarks ...
*/

Ibo::~Ibo()
{
  for (unsigned i=0u; i!=GLOOST_SYSTEM_NUM_RENDER_CONTEXTS; ++i)
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
Ibo::initInContext (unsigned contextId)
{

//  CheckErrorsGL("Before gloost::Ibo::initInContext()");

  // Triangles indices
  unsigned indexBufferHandle = 0u;

  glGenBuffers(1, &indexBufferHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);

  // element array for quads
  if (_mesh->getQuads().size())
  {
    _suggestedDrawMode = GL_QUADS;
    _numIdices = _mesh->getQuads().size() * 4;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _numIdices * sizeof(unsigned),
                 &(_mesh->getQuads().front()),
                 GL_STATIC_DRAW);

  }
  // element array for triangles
  else if (_mesh->getTriangles().size())
  {
    _suggestedDrawMode = GL_TRIANGLES;
    _numIdices = _mesh->getTriangles().size() * 3;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _numIdices * sizeof(unsigned),
                 &(_mesh->getTriangles().front()),
                 GL_DYNAMIC_DRAW);

  }
  // element array for lines
  else if (_mesh->getLines().size())
  {
    _suggestedDrawMode = GL_LINES;
    _numIdices = _mesh->getLines().size() * 2;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _numIdices * sizeof(unsigned),
                 &(_mesh->getLines().front()),
                 GL_STATIC_DRAW);


  }
  // element array for points
  else if (_mesh->getPoints().size())
  {
    _suggestedDrawMode = GL_POINTS;
    _numIdices = _mesh->getPoints().size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _numIdices * sizeof(unsigned),
                 &(_mesh->getPoints().front()),
                 GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  setResourceHandleForContext(indexBufferHandle,
                              contextId,
                              0);

//  CheckErrorsGL("After gloost::Ibo::initInContext()");


  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Removes a resource from a context
  \remarks ...
*/

/*virtual*/
void
Ibo::removeFromContext(unsigned contextId)
{
  if (handleInContextExists(contextId, 0))
  {
    glDeleteBuffers(1, &getResourceHandleForContext(contextId, 0));
    setResourceHandleForContext(0, contextId);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   binds the drawable into state by specify its components
  \remarks ...
*/

/*virtual*/
void
Ibo::bind(unsigned contextId)
{
  if (!handleInContextExists(contextId, 0))
  {
    initInContext(contextId);
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getResourceHandleForContext(contextId, 0));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief removes this drawable from state
  \remarks ...
*/

/*virtual*/
void
Ibo::unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   draws the ibo
  \remarks ...
*/

/*virtual*/
void
Ibo::draw()
{
  if (_drawMode == GLOOST_IBO_SUGGESTED_DRAWMODE)
  {
    glDrawElements(_suggestedDrawMode, _numIdices, GL_UNSIGNED_INT, 0);
    return;
  }

  glDrawElements(_drawMode, _numIdices, GL_UNSIGNED_INT, 0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   draws the ibo for a custom drawMode
  \remarks ...
*/

/*virtual*/
void
Ibo::draw(unsigned drawMode)
{
//  if (_drawMode == GLOOST_IBO_SUGGESTED_DRAWMODE)
//  {
//    glDrawElements(_suggestedDrawMode, _numIdices, GL_UNSIGNED_INT, 0);
//    return;
//  }

  glDrawElements(drawMode, _numIdices, GL_UNSIGNED_INT, 0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief sets the draw mode
  \remarks draw mode could be GL_TRIANGLES_GL_POINTS_GL_LINESTRIP, GL_PATCHES, ...
*/

/*virtual*/
void
Ibo::setDrawMode(unsigned drawMode)
{
  _drawMode = drawMode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the draw mode
  \remarks ...
*/

/*virtual*/
unsigned
Ibo::getDrawMode()
{
  return _drawMode;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Draws only Faces (for shadowmap pass for example)
  \remarks ...
*/
///*virtual*/
//void
//Ibo::draw(int startIndex, int endIndex, unsigned contextId)
//{
//  if (_drawMode == GLOOST_IBO_SUGGESTED_DRAWMODE)
//  {
//    glDrawRangeElements(_suggestedDrawMode,
//                        startIndex,
//                        endIndex,
//                        (int)endIndex-(int)startIndex,
//                        GL_UNSIGNED_INT,
//                        0);
//    return;
//  }
//
//  glDrawRangeElements(_drawMode,
//                      startIndex,
//                      endIndex,
//                      (int)endIndex-(int)startIndex,
//                      GL_UNSIGNED_INT,
//                      0);
//}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the VertexAttribContainer used by this Ibo
  \remarks ...
*/

const std::shared_ptr<const Mesh>&
Ibo::getMesh() const
{
  return _mesh;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the number of indices
  \remarks ...
*/

const unsigned&
Ibo::getNumIndices() const
{
  return _numIdices;
}

////////////////////////////////////////////////////////////////////////////////



} // namespace gl

} // namespace gloost
