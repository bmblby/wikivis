
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
#include <gloost/obsolete/Vbo.h>
#include <gloost/gloostConfig.h>
#include <gloost/glErrorUtil.h>



// cpp includes
#include <string>
#include <iostream>
#include <GL/glew.h>



namespace gloost
{


namespace obsolete
{

/**
  \class Vbo

  \brief Wrapper for an OpenGL Vertex Buffer Object.

  \author Felix Weiszig
  \date   February 2011
  \remarks This class build an interleaved vertex array with an index buffer from the
           available components of a gloost::Mesh. Triangle faces, lines and points
           are supported.
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

Vbo::Vbo(std::shared_ptr<Mesh> mesh):
  Drawable(GLOOST_VBO_NUM_RESOURCES, mesh),
  _elementArraySize(0),
  _supportedInterleavedComponents(GLOOST_BITMASK_ALL_UNSET),
  _interleavedInfo()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
  \remarks ...
*/

Vbo::~Vbo()
{
  if (getResourceHandleForContext(0, GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY))
  {
    glDeleteBuffers(1, &getResourceHandleForContext(0, GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY));
  }

  if (getResourceHandleForContext(0, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY))
  {
    glDeleteBuffers(1, &getResourceHandleForContext(0, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY));
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief initialize the resource within a context
  \remarks ...
*/

bool
Vbo::initInContext (unsigned int contextId)
{
  std::shared_ptr<Mesh> interleavedMesh = nullptr;

  if (_mesh->getInterleavedAttributes().size())
  {
    // mesh is allready interleaved
    interleavedMesh = _mesh;
  }
  else
  {
    // use another mesh to interleave attributes
    interleavedMesh = Mesh::create(_mesh, true);
  }

//  interleavedMesh->printMeshInfo();

  _supportedInterleavedComponents = BitMask(interleavedMesh->getSupportedInterleavedAttributes());
  _interleavedInfo                = interleavedMesh->getInterleavedInfo();


//  CheckErrorsGL("Vbo befor building");


  // Handle for interleaved array
  GLuint vertexBufferHandle = 0;

  // generate a new VBO and get the associated ID
  glGenBuffers(1, &vertexBufferHandle);

  // bind VBO in order to use
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);

  // upload data to VBO
  glBufferData(GL_ARRAY_BUFFER,
               interleavedMesh->getInterleavedAttributes().size() * sizeof(float),
               &(interleavedMesh->getInterleavedAttributes().front()),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER,0);


  setResourceHandleForContext(vertexBufferHandle,
                              contextId,
                              GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY);



//  glEnableClientState(GL_VERTEX_ARRAY);
//  glBindBuffer(GL_ARRAY_BUFFER, getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY));
//  glVertexPointer(3, GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedVertexStride));
//
//  if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_NORMALS))
//  {
//    glEnableClientState(GL_NORMAL_ARRAY);
//    glNormalPointer(GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedNormalStride));
//  }
//
//  if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_COLORS))
//  {
//    glEnableClientState(GL_COLOR_ARRAY);
//    glColorPointer(4, GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedColorStride));
//  }
//
//  if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_TEXCOORDS))
//  {
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    glTexCoordPointer(2, GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedTexcoordStride));
//  }


  // Triangles indices
  GLuint elementBufferHandle = 0;

  glGenBuffers(1, &elementBufferHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferHandle);

  // element array for quads
  if (interleavedMesh->getQuads().size())
  {
    _suggestedDrawMode = GL_QUADS;

//    std::vector<Mesh::QuadIndices>& srcQuads = interleavedMesh->getQuads();

    _elementArraySize = interleavedMesh->getQuads().size() * 4;

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _elementArraySize * sizeof(unsigned int),
                 &(interleavedMesh->getQuads().front()),
                 GL_STATIC_DRAW);

  }
  // element array for triangles
  else if (interleavedMesh->getTriangles().size())
  {
    _suggestedDrawMode = GL_TRIANGLES;

    _elementArraySize = interleavedMesh->getTriangles().size() * 3;

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _elementArraySize * sizeof(unsigned int),
                 &(interleavedMesh->getTriangles().front()),
                 GL_DYNAMIC_DRAW);

  }
  // element array for lines
  else if (interleavedMesh->getLines().size())
  {
    _suggestedDrawMode = GL_LINES;
    _elementArraySize = interleavedMesh->getLines().size() * 2;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _elementArraySize * sizeof(unsigned int),
                 &(interleavedMesh->getLines().front()),
                 GL_STATIC_DRAW);


  }
  // element array for points
  else if (interleavedMesh->getPoints().size())
  {
    _suggestedDrawMode = GL_POINTS;

    _elementArraySize = interleavedMesh->getPoints().size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _elementArraySize * sizeof(unsigned int),
                 &(interleavedMesh->getPoints().front()),
                 GL_STATIC_DRAW);


  }
//  else
//  {
//    _suggestedDrawMode = GL_POINTS;
//  }


  if (_drawMode == GLOOST_DRAWABLE_SUGGESTED_DRAWMODE)
  {
    _drawMode = _suggestedDrawMode;
  }



  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

  setResourceHandleForContext(elementBufferHandle,
                              contextId, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY);


//  std::cerr << std::endl << "vertexBufferHandle: " << vertexBufferHandle;

//  CheckErrorsGL("Vbo after init");
  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Removes a resource from a context
  \remarks ...
*/

/*virtual*/
void
Vbo::removeFromContext(unsigned int contextId)
{

  if (handleInContextExists(contextId, GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY))
  {
    glDeleteBuffersARB(1, &getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY));
  }
  if (handleInContextExists(contextId, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY))
  {
    glDeleteBuffersARB(1, &getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY));
  }

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief binds the drawable into state by specify its components
  \remarks ...
*/

/*virtual*/
void
Vbo::bind(unsigned int, unsigned int contextId)
{


//  CheckErrorsGL("befor bind in -> gloost::obsolete::Vbo::bind(unsigned int supportIndexRanges, unsigned int contextId)");

  if (!handleInContextExists(contextId, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY))
  {
    initInContext(contextId);
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY));
  glVertexPointer(GLOOST_MESH_NUM_COMPONENTS_VERTEX, GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedVertexStride));

  if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_NORMALS))
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedNormalStride));
  }

  if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_COLORS))
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(GLOOST_MESH_NUM_COMPONENTS_COLOR, GL_FLOAT, _interleavedInfo.interleavedPackageStride, BUFFER_OFFSET(_interleavedInfo.interleavedColorStride));
  }

  if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_TEXCOORDS))
  {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(GLOOST_MESH_NUM_COMPONENTS_TEXCOORD,
                      GL_FLOAT,
                      _interleavedInfo.interleavedPackageStride,
                      BUFFER_OFFSET(_interleavedInfo.interleavedTexcoordStride));
  }


//  CheckErrorsGL("after bind in -> gloost::obsolete::Vbo::bind(unsigned int supportIndexRanges, unsigned int contextId)");

  // for indices
//  if (!supportIndexRanges)
  {
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, getResourceHandleForContext(contextId, GLOOST_VBO_RESOURCE_ELEMENT_ARRAY));
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief removes this drawable from state
  \remarks ...
*/

/*virtual*/
void
Vbo::disable(unsigned int)
{
  // deactivate vertex array
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Draws only Faces (for shadowmap pass for example)
  \remarks ...
*/

/*virtual*/
void
Vbo::draw(unsigned int)
{

  // Mesh was empty
  if (_drawMode == GLOOST_DRAWABLE_SUGGESTED_DRAWMODE)
  {
    return;
  }


//  CheckErrorsGL("befor glDrawElements in -> gloost::obsolete::Vbo::draw(unsigned int contextId)");



//#if 1
//  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDrawElements(_drawMode, _elementArraySize, GL_UNSIGNED_INT, 0);
//#else
//  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
//  IndexRange range = _mesh->getIndexRanges()["ring"];
//  glDrawElements(_drawMode, range.elements.size(), GL_UNSIGNED_INT, &range.elements.front());
//#endif


//  CheckErrorsGL("after glDrawElements in -> gloost::obsolete::Vbo::draw(unsigned int contextId)");

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Increments the reference counter
*/

/*virtual*/
Vbo*
Vbo::takeReference()
{
	Drawable::takeReference();
	return this;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Decrements the reference counter
*/

/*virtual*/
void
Vbo::dropReference()
{
	Drawable::dropReference();
}


////////////////////////////////////////////////////////////////////////////////



} // namespace obsolete
} // namespace gloost
