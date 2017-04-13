
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
#include <gloost/Mesh.h>
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/gloostMath.h>
#include <gloost/Matrix.h>
#include <gloost/BinaryBundle.h>



// cpp includes
#include <string>
#include <iostream>
#include <vector>


namespace gloost
{

/**
  \class Mesh

  \brief Mesh container storing vertices, normals, texture coordinates, triangles, quads, lines

  \author Felix Weiszig
  \date   Feb 2011
  \remarks
*/


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class factory
*/

/*static*/
Mesh::shared_ptr
Mesh::create()
{
  return shared_ptr(new Mesh());
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class factory
*/

/*static*/
Mesh::shared_ptr
Mesh::create(const_shared_ptr mesh, bool interleave)
{
  return shared_ptr(new Mesh(mesh, interleave));
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
*/

Mesh::Mesh():
  _positions(),
  _normals(),
  _colors(),
  _texCoords(),
  _interleavedAttributes(),
  _points(),
  _lines(),
  _triangles(),
  _quads(),
  _materialIds(),
  _supportedInterleavedComponents(GLOOST_BITMASK_ALL_UNSET),
  _interleavedInfo(),
  _indexRanges(),
  _boundingBox()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param mesh A gloost::Mesh container
  \param interleave determines if the copy will be interleaved
*/

Mesh::Mesh(const_shared_ptr mesh, bool interleave):
  _positions(),
  _normals(),
  _colors(),
  _texCoords(),
  _interleavedAttributes(),
  _points(),
  _lines(),
  _triangles(),
  _quads(),
  _supportedInterleavedComponents(GLOOST_BITMASK_ALL_UNSET),
  _interleavedInfo(),
  _indexRanges(),
  _boundingBox(mesh->_boundingBox.getPMin(), mesh->_boundingBox.getPMax())
{
  _interleavedAttributes          = mesh->_interleavedAttributes;
  _supportedInterleavedComponents = mesh->_supportedInterleavedComponents;

  if (interleave)
  {
    interleaveFromOtherMesh(mesh);
  }
  else
  {
    _positions   = mesh->_positions;
    _normals    = mesh->_normals;
    _colors     = mesh->_colors;
    _texCoords  = mesh->_texCoords;
  }

  _points      = mesh->_points;
  _lines       = mesh->_lines;
  _triangles   = mesh->_triangles;
  _quads       = mesh->_quads;
  _materialIds = mesh->_materialIds;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class destructor
*/

Mesh::~Mesh()
{
//  clear();
//  std::cerr << std::endl << "Mesh::~Mesh(): " << this;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of vertices

  \remarks ...
*/

std::vector<Point3>&
Mesh::getPositions()
{
  return _positions;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of vertices

  \remarks ...
*/

const std::vector<Point3>&
Mesh::getPositions() const
{
  return _positions;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of normals
*/

std::vector<Vector3>&
Mesh::getNormals()
{
  return _normals;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of normals
*/

const std::vector<Vector3>&
Mesh::getNormals() const
{
  return _normals;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of colors
*/

std::vector<vec4>&
Mesh::getColors()
{
  return _colors;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of colors
*/

const std::vector<vec4>&
Mesh::getColors() const
{
  return _colors;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of texCoords
  \remarks ...
*/

std::vector<Point3>&
Mesh::getTexCoords()
{
  return _texCoords;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of texCoords
  \remarks ...
*/

const std::vector<Point3>&
Mesh::getTexCoords() const
{
  return _texCoords;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of interleaved components
  \remarks ...
*/

std::vector<float>&
Mesh::getInterleavedAttributes()
{
  return _interleavedAttributes;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of interleaved components
  \remarks ...
*/

const std::vector<float>&
Mesh::getInterleavedAttributes() const
{
  return _interleavedAttributes;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief interleaves the vertex positions with available vertex components
  \remarks ...
*/

void
Mesh::interleave(bool clearAttributeArraysAfterwards)
{

  // test if mesh is allready interleaved
  if (!_positions.size())
  {
    return;
  }

  interleaveFromOtherMesh(shared_from_this());

  // clear components
  if (clearAttributeArraysAfterwards)
  {
    if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_POSITIONS))
    {
      _positions.clear();
    }
    if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_NORMALS))
    {
      _normals.clear();
    }
    if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_COLORS))
    {
      _colors.clear();
    }
    if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_TEXCOORDS))
    {
      _texCoords.clear();
    }
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief interleaves a meshs vertex positions with available vertex components
  \remarks ...
*/

void
Mesh::interleaveFromOtherMesh(const_shared_ptr mesh)
{
  _supportedInterleavedComponents.setFlags(gloost::BitMask(GLOOST_BITMASK_ALL_SET), false);

  // copy indices
  _points      = mesh->_points;
  _lines       = mesh->_lines;
  _triangles   = mesh->_triangles;
  _quads       = mesh->_quads;
  _materialIds = mesh->_materialIds;


  // if input mesh is allready interleaved
  if (mesh->getInterleavedAttributes().size())
  {
    _interleavedAttributes          = mesh->_interleavedAttributes;
    _supportedInterleavedComponents = mesh->_supportedInterleavedComponents;
    return;
  }

  /// if input mesh is NOT interleaved
  if (mesh->_positions.size())
  {
    setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS, true);

    // check for per vertex attributes, use only if per vertex attribute is available
    if (mesh->_positions.size() == mesh->_normals.size())
    {
      setSupportedInterleavedAttribute(GLOOST_MESH_NORMALS, true);
    }
    if (mesh->_positions.size() == mesh->_colors.size())
    {
      setSupportedInterleavedAttribute(GLOOST_MESH_COLORS, true);
    }
    if (mesh->_positions.size() == mesh->_texCoords.size())
    {
      setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS, true);
    }

    _interleavedAttributes.clear();

    // interleave attributes
    for (unsigned i=0; i!=mesh->_positions.size(); ++i)
    {
      _interleavedAttributes.push_back(mesh->_positions[i][0]);
      _interleavedAttributes.push_back(mesh->_positions[i][1]);
      _interleavedAttributes.push_back(mesh->_positions[i][2]);

      if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_NORMALS))
      {
        _interleavedAttributes.push_back(mesh->_normals[i][0]);
        _interleavedAttributes.push_back(mesh->_normals[i][1]);
        _interleavedAttributes.push_back(mesh->_normals[i][2]);
      }
      if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_COLORS))
      {
        _interleavedAttributes.push_back(mesh->_colors[i].r);
        _interleavedAttributes.push_back(mesh->_colors[i].g);
        _interleavedAttributes.push_back(mesh->_colors[i].b);
        _interleavedAttributes.push_back(mesh->_colors[i].a);
      }
      if (_supportedInterleavedComponents.getFlag(GLOOST_MESH_TEXCOORDS))
      {
        _interleavedAttributes.push_back(mesh->_texCoords[i][0]);
        _interleavedAttributes.push_back(mesh->_texCoords[i][1]);
      }
    }
  }

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of Points
  \remarks ...
*/

std::vector<unsigned int>&
Mesh::getPoints()
{
  return _points;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of Points
  \remarks ...
*/

const std::vector<unsigned int>&
Mesh::getPoints() const
{
  return _points;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of lines
  \remarks ...
*/

std::vector<Mesh::LineIndices>&
Mesh::getLines()
{
  return _lines;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of lines
  \remarks ...
*/

const std::vector<Mesh::LineIndices>&
Mesh::getLines() const
{
  return _lines;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of triangles
  \remarks ...
*/

std::vector<Mesh::TriangleIndices>&
Mesh::getTriangles()
{
  return _triangles;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of triangles
  \remarks ...
*/

const std::vector<Mesh::TriangleIndices>&
Mesh::getTriangles() const
{
  return _triangles;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of quads
  \remarks ...
*/

std::vector<Mesh::QuadIndices>&
Mesh::getQuads()
{
  return _quads;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of quads
  \remarks ...
*/

const std::vector<Mesh::QuadIndices>&
Mesh::getQuads() const
{
  return _quads;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the vector of material ids
  \remarks ...
*/

std::vector<unsigned>&
Mesh::getMaterialIds()
{
  return _materialIds;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the number of interleaved packages of vertices and their attributes
  \remarks ...
*/

unsigned int
Mesh::getNumInterleavedPackages() const
{
  if(_interleavedInfo.interleavedPackageSize)
  {
    return _interleavedAttributes.size() / _interleavedInfo.interleavedPackageSize;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the memory cosumtion of this mesh in cpu ram in byte
  \remarks ...
*/

float
Mesh::getMemoryUsage() const
{
  return    _positions.size()*sizeof(gloost::Point3)
          + _normals.size()*sizeof(gloost::Vector3)
          + _colors.size()*sizeof(gloost::vec4)
          + _texCoords.size()*sizeof(gloost::Point3)
          + _interleavedAttributes.size()*sizeof(float)
          + _points.size()*sizeof(unsigned)
          + _lines.size()*sizeof(LineIndices)
          + _triangles.size()*sizeof(TriangleIndices)
          + _quads.size()*sizeof(QuadIndices)
          + _materialIds.size()*sizeof(unsigned);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns a map <std::string, IndexRage> with ranges within an element array
  \remarks ...
*/

std::map<std::string, IndexRange>&
Mesh::getIndexRanges()
{
  return _indexRanges;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief replaces the interleaved attributes with the content of a BinaryBundle
  \remarks ...
*/

void
Mesh::setInterleavedAttribs(const BinaryBundle* attribBundle)
{
  if (attribBundle->getSize() % sizeof(float) != 0)
  {
    std::cerr << std::endl << "attribBundle->getSize() % sizeof(float) != 0: ";
    return;
  }

  _interleavedAttributes.resize(attribBundle->getSize()/sizeof(float));
  memcpy((unsigned char*)&_interleavedAttributes.front(), attribBundle->data(), attribBundle->getSize());
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief push interleaved component into the container
  \remarks ...
*/

void
Mesh::pushInterleavedComponent(const Point3& pointOrTexCoord)
{
  _interleavedAttributes.push_back(pointOrTexCoord[0]);
  _interleavedAttributes.push_back(pointOrTexCoord[1]);
  _interleavedAttributes.push_back(pointOrTexCoord[2]);
}

///////////////////////////////////////////////////////////////////////////////



/**
  \brief push interleaved component into the container
  \remarks ...
*/

void
Mesh::pushInterleavedComponent(const Vector3& normal)
{
  _interleavedAttributes.push_back(normal[0]);
  _interleavedAttributes.push_back(normal[1]);
  _interleavedAttributes.push_back(normal[2]);
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief push interleaved component into the container
  \remarks ...
*/

void
Mesh::pushInterleavedComponent(const vec4& color)
{
  _interleavedAttributes.push_back(color.r);
  _interleavedAttributes.push_back(color.g);
  _interleavedAttributes.push_back(color.b);
  _interleavedAttributes.push_back(color.a);
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief push interleaved component into the container
  \remarks ...
*/

void
Mesh::pushInterleavedComponent(const vec2& texcoord)
{
  _interleavedAttributes.push_back(texcoord.u);
  _interleavedAttributes.push_back(texcoord.v);
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief push interleaved component into the container
  \remarks ...
*/

void
Mesh::pushInterleavedComponent(float scalar)
{
  _interleavedAttributes.push_back(scalar);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns a BitMask indicating available interleaved attributes
  \remarks ...
*/

const BitMask&
Mesh::getSupportedInterleavedAttributes() const
{
  return _supportedInterleavedComponents;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief enables or disables a available interleaved attribute
  \remarks ...
*/

void
Mesh::setSupportedInterleavedAttribute(unsigned attribute, bool state)
{
  _supportedInterleavedComponents.setFlag(attribute, state);

  updateMeshInfo();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns true if attribute is enabled for interleaved data
  \remarks ...
*/

bool
Mesh::isInterleavedAttributeSupported(unsigned attribute) const
{
  return _supportedInterleavedComponents.getFlag(attribute);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the MeshInfo containing information about interleaved data
  \remarks ...
*/

const MeshInterleavedInfo&
Mesh::getInterleavedInfo() const
{
  return _interleavedInfo;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Transforms vertex and normal data with a matrix
  \param matrix A transformation matrix
  \remarks ...
*/

void
Mesh::transform(const Matrix& matrix, bool normalizeNormalVectors)
{

  // vertices
  for (unsigned i=0; i!=_positions.size(); ++i)
  {
    _positions[i] = matrix * _positions[i];
  }

  // normals

//  gloost::Matrix normalMatrix = matrix.inverted().transposed();

  for (unsigned i=0; i!=_normals.size(); ++i)
  {
    _normals[i] = matrix * _normals[i];
  }



  // for interleaved data
  if (_interleavedAttributes.size())
  {
    // size of one interleaved vertex + attributes
    int indexStepWidth = _interleavedInfo.interleavedPackageSize;
    int vertexPos      = _interleavedInfo.interleavedVertexOffset;
    int normalPos      = _interleavedInfo.interleavedNormalOffset;;


    // positions
    for (unsigned i=vertexPos; i<_interleavedAttributes.size(); )
    {

      gloost::Point3 vertex(_interleavedAttributes[i],    // x
                            _interleavedAttributes[i+1],  // y
                            _interleavedAttributes[i+2]); // z

      vertex = matrix * vertex;

      _interleavedAttributes[i]   = vertex[0];
      _interleavedAttributes[i+1] = vertex[1];
      _interleavedAttributes[i+2] = vertex[2];

      i += indexStepWidth;
    }

    if (normalPos)
    {
      // normals
      for (unsigned i=normalPos; i<_interleavedAttributes.size(); )
      {

        gloost::Vector3 normal(_interleavedAttributes[i],    // x
                               _interleavedAttributes[i+1],  // y
                               _interleavedAttributes[i+2]); // z

        normal = matrix * normal;

        _interleavedAttributes[i]   = normal[0];
        _interleavedAttributes[i+1] = normal[1];
        _interleavedAttributes[i+2] = normal[2];

        i += indexStepWidth;
      }
    }
  }


  // bounding box
  recalcBoundingBox();

  // normalize normals
  if (normalizeNormalVectors)
  {
    normalizeNormals();
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief normalizes normal vectors
  \remarks ...
*/

void
Mesh::normalizeNormals()
{
  for (unsigned i=0; i!=_normals.size(); ++i)
  {
    _normals[i].normalize();
  }


  // for interleaved data
  if (_interleavedAttributes.size())
  {
    // size of one interleaved vertex + attributes
    int indexStepWidth = _interleavedInfo.interleavedPackageSize; // add size of vertex
    int normalPos      = _interleavedInfo.interleavedNormalOffset;

    if (normalPos)
    {
      for (unsigned i=normalPos; i<_interleavedAttributes.size(); )
      {

        gloost::Vector3 normal(_interleavedAttributes[i],    // x
                               _interleavedAttributes[i+1],  // y
                               _interleavedAttributes[i+2]); // z

        normal.normalize();

        _interleavedAttributes[i]   = normal[0];
        _interleavedAttributes[i+1] = normal[1];
        _interleavedAttributes[i+2] = normal[2];

        i += indexStepWidth;
      }
    }
  }

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Generates one normal per face by averaging the participating vertex normals
  \remarks ...
*/

//void
//Mesh::generateFlatShadingFaceNormals()
//{
//
//  std::vector<Vector3> flatShadingNormals;
//
//  if (_positions.size() && _positions.size() == _normals.size())
//  {
//    for (unsigned f=0; f!=_triangles.size(); ++f)
//    {
//      unsigned int* nIndex = _triangles[f].vertexIndices;
//
//      Vector3 normal0 = _normals[nIndex[0]];
//      Vector3 normal1 = _normals[nIndex[1]];
//      Vector3 normal2 = _normals[nIndex[2]];
//
//      normal0.normalize();
//      normal1.normalize();
//      normal2.normalize();
//
//      Vector3 flatFaceNormal = (normal0+normal1+normal2) / 3.0;
//      flatFaceNormal.normalize();
//
//      nIndex[0] = flatShadingNormals.size();
//      nIndex[1] = flatShadingNormals.size();
//      nIndex[2] = flatShadingNormals.size();
//      flatShadingNormals.push_back(flatFaceNormal);
//    }
//
//    _normals = flatShadingNormals;
//  }
//
//}


/////////////////////////////////////////////////////////////////////////////


/**
  \brief   Generates per vertex normals from face vertices
  \remarks This will only work if the mesh is NOT interleaved
*/

void
Mesh::generateNormals()
{

  if (!_positions.size())
  {
    return;
  }

  _normals.clear();
  _normals = std::vector<Vector3>(_positions.size(), Vector3(0.0,1.0,0.0));

  std::vector<unsigned> numValuesPerComponent(_normals.size(), 0u);

  for (unsigned i=0; i!=_triangles.size(); ++i)
  {
    unsigned int* vindex = _triangles[i].vertexIndices;

    Point3 vertex0 = _positions[vindex[0]];
    Point3 vertex1 = _positions[vindex[1]];
    Point3 vertex2 = _positions[vindex[2]];

    Vector3 a( (vertex1 - vertex0).normalized() );
    Vector3 b( (vertex2 - vertex1).normalized() );
    Vector3 c( (vertex0 - vertex2).normalized() );

    Vector3 n0 = cross(c, a).normalized();
    Vector3 n1 = cross(a, b).normalized();
    Vector3 n2 = cross(b, c).normalized();

    _normals[vindex[0]] += n0;
    _normals[vindex[1]] += n1;
    _normals[vindex[2]] += n2;
  }

  normalizeNormals();
}


/////////////////////////////////////////////////////////////////////////////


/**
  \brief Centers the Mesh by altering vertex positons
  \remarks ...
*/

void
Mesh::center()
{
  Matrix offset;
  offset.setIdentity();
  offset.setTranslate(getOffsetToCenter());

  transform(offset, false);

  _boundingBox.transform(offset);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the offset to the centered position
  \remarks ...
*/

Vector3
Mesh::getOffsetToCenter()
{
  Point3 min = _boundingBox.getPMin();
  Point3 max = _boundingBox.getPMax();

  return (min+max)*-0.5;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief scales the Mesh so that the longest side of the bounding box is of size maxLength
  \remarks ...
*/

void
Mesh::scaleToSize(mathType maxLength)
{
  Matrix scaleMat;
  scaleMat.setIdentity();

  scaleMat.setScale(getScaleFactorToSize(maxLength));

  transform(scaleMat, false);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns a factor to scale the mesh by, so the longest side of the
         bounding box is of size maxlength
  \remarks ...
*/

mathType
Mesh::getScaleFactorToSize(mathType maxLength)
{
  Point3 min = _boundingBox.getPMin();
  Point3 max = _boundingBox.getPMax();

  Vector3 size(std::abs(max[0] - min[0]),
               std::abs(max[1] - min[1]),
               std::abs(max[2] - min[2]));

  unsigned mainAxisIndex = size.getMainAxis();


  mathType biggestValue = size[mainAxisIndex];

  if (std::abs(biggestValue) > 0.0)
  {
    return maxLength/biggestValue;
  }

  return 1.0;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   normalizes the colors from 0...maxValue to 0...1
  \remarks ...
*/

void
Mesh::convertVertexColorComponentsToFloat(float maxValue)
{

  if (!maxValue)
  {
    return;
  }

  float factor = 1.0f/maxValue;

  for (unsigned i=0; i!=_colors.size(); ++i)
  {
    _colors[i].r *= factor;
    _colors[i].g *= factor;
    _colors[i].b *= factor;
    _colors[i].a *= factor;
  }




  // for interleaved data
  if (_interleavedAttributes.size())
  {
    // size of one interleaved vertex + attributes
    int indexStepWidth = _interleavedInfo.interleavedPackageSize;
    int colorPos       = _interleavedInfo.interleavedColorOffset;

    if (colorPos)
    {
      for (unsigned i=colorPos; i<_interleavedAttributes.size(); )
      {
        _interleavedAttributes[i]   *= factor; // red
        _interleavedAttributes[i+1] *= factor; // green
        _interleavedAttributes[i+2] *= factor; // blue
        _interleavedAttributes[i+3] *= factor; // alpha

        i += indexStepWidth;
      }
    }
  }

}


///////////////////////////////////////////////////////////////////////////////

///**
//  \brief   merges vertices if they have same position
//  \remarks ...
//*/
//
//void
//Mesh::mergeVertices(float maxDist)
//{
//
//  std::cerr << std::endl << "Mesh::mergeVertices(float maxDist): " << _positions.size();
//
//  std::vector<int> mergePositionLookUp(_positions.size(), -1);
//
//  for (unsigned a=0; a!=_positions.size(); ++a)
//  {
//    for (unsigned b=0; b!=_positions.size(); ++b)
//    {
//      if (a != b)
//      {
//        if ( Vector3(_positions[a] - _positions[b]).length2() <= maxDist )
//        {
//          mergePositionLookUp[b] = a;
//        }
//      }
//    }
//
//
//    if (a%500 == 0)
//    std::cerr << std::endl << "a: " << a;
//  }
//}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a Mesh to this one
  \remarks 1. To work properly both meshes have to have same attributes </br<
           2. You have to call recalcBoundingBox() after add(), otherwise transform() and other
           operations will not work properly
*/


void
Mesh::add(std::shared_ptr<Mesh> mesh)
{

  unsigned interleavePackageOffset = getNumInterleavedPackages();
  unsigned verticesOffset          = getPositions().size();

  _positions.insert            ( _positions.end(), mesh->getPositions().begin(), mesh->getPositions().end() );
  _normals.insert              ( _normals.end(),   mesh->getNormals().begin(), mesh->getNormals().end() );
  _colors.insert               ( _colors.end(),    mesh->getColors().begin(), mesh->getColors().end() );
  _texCoords.insert            ( _texCoords.end(), mesh->getTexCoords().begin(), mesh->getTexCoords().end() );
  _interleavedAttributes.insert( _interleavedAttributes.end(), mesh->getInterleavedAttributes().begin(), mesh->getInterleavedAttributes().end() );

  int offsetToUse;

  if (interleavePackageOffset)
  {
    offsetToUse = interleavePackageOffset;
  }
  else
  {
    offsetToUse = verticesOffset;
  }

  for (unsigned i=0; i!=mesh->getPoints().size(); ++i)
  {
    _points.push_back(mesh->getPoints()[i] + offsetToUse);
  }

  for (unsigned i=0; i!=mesh->getLines().size(); ++i)
  {
    auto line = mesh->getLines()[i];
    line.vertexIndices[0] += offsetToUse;
    line.vertexIndices[1] += offsetToUse;
    _lines.push_back(line);
  }

  for (unsigned i=0; i!=mesh->getTriangles().size(); ++i)
  {
    auto tri = mesh->getTriangles()[i];
    tri.vertexIndices[0] += offsetToUse;
    tri.vertexIndices[1] += offsetToUse;
    tri.vertexIndices[2] += offsetToUse;
    _triangles.push_back(tri);

  }

  for (unsigned i=0; i!=mesh->getQuads().size(); ++i)
  {
    auto quad = mesh->getQuads()[i];
    quad.vertexIndices[0] += offsetToUse;
    quad.vertexIndices[1] += offsetToUse;
    quad.vertexIndices[2] += offsetToUse;
    quad.vertexIndices[3] += offsetToUse;
    _quads.push_back(quad);
  }

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns BoundingBox of this Mesh
  \remarks ...
*/

BoundingBox&
Mesh::getBoundingBox()
{
  return _boundingBox;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns BoundingBox of this Mesh
  \remarks ...
*/

const BoundingBox&
Mesh::getBoundingBox() const
{
  return _boundingBox;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief recalculates BoundingBox
  \remarks ...
*/

void
Mesh::recalcBoundingBox()
{
  Point3 min;
  Point3 max;

  if (_positions.size())
  {
    min = _positions[0];
    max = _positions[0];

    for(unsigned i=0; i!=_positions.size(); ++i)
    {
      if (_positions[i][0] < min[0]) min[0] = _positions[i][0];
      if (_positions[i][1] < min[1]) min[1] = _positions[i][1];
      if (_positions[i][2] < min[2]) min[2] = _positions[i][2];

      if (_positions[i][0] > max[0]) max[0] = _positions[i][0];
      if (_positions[i][1] > max[1]) max[1] = _positions[i][1];
      if (_positions[i][2] > max[2]) max[2] = _positions[i][2];
    }

    _boundingBox.setPMin(min);
    _boundingBox.setPMax(max);
    return;
  }


  // for interleaved data
  if (_interleavedAttributes.size() >= 3 )
  {
    // size of one interleaved vertex + attributes
    int indexStepWidth = _interleavedInfo.interleavedPackageSize; // add size of vertex
    int vertexPos      = _interleavedInfo.interleavedVertexOffset;

    min[0] = _interleavedAttributes[vertexPos + 0];
    min[1] = _interleavedAttributes[vertexPos + 1];
    min[2] = _interleavedAttributes[vertexPos + 2];
    max[0] = _interleavedAttributes[vertexPos + 0];
    max[1] = _interleavedAttributes[vertexPos + 1];
    max[2] = _interleavedAttributes[vertexPos + 2];

//    if (vertexPos)
    {

      for (unsigned i=vertexPos; i<_interleavedAttributes.size(); )
      {
        Point3 vertex(_interleavedAttributes[i],
                      _interleavedAttributes[i+1],
                      _interleavedAttributes[i+2]);

        i += indexStepWidth;


        if (vertex[0] < min[0]) min[0] = vertex[0] ;
        if (vertex[1] < min[1]) min[1] = vertex[1];
        if (vertex[2] < min[2]) min[2] = vertex[2];

        if (vertex[0] > max[0]) max[0] = vertex[0];
        if (vertex[1] > max[1]) max[1] = vertex[1];
        if (vertex[2] > max[2]) max[2] = vertex[2];
      }
    }
  }

  _boundingBox.setPMin(min);
  _boundingBox.setPMax(max);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief clears the mesh
  \remarks ...
*/

void
Mesh::clear()
{
  decltype(_positions)().swap(_positions);
  _normals.clear();
  _colors.clear();
  _texCoords.clear();
  _interleavedAttributes.clear();
  _points.clear();
  _lines.clear();
  _triangles.clear();
  _quads.clear();
  _materialIds.clear();
  _indexRanges.clear();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns true if Mesh is empty

  \remarks ...
*/

bool
Mesh::isEmpty() const
{
  return !(bool) (_positions.size() +
                  _normals.size() +
                  _colors.size() +
                  _texCoords.size() +
                  _interleavedAttributes.size() +
                  _points.size() +
                  _lines.size() +
                  _triangles.size() +
                  _quads.size());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief updates the mesh info
  \remarks ...
*/

void
Mesh::updateMeshInfo()
{

  _interleavedInfo = MeshInterleavedInfo();

  unsigned componentOffset = 0;
  unsigned componentStride = 0;

  if (isInterleavedAttributeSupported(GLOOST_MESH_POSITIONS))
  {
    _interleavedInfo.interleavedVertexOffset = componentOffset;
    _interleavedInfo.interleavedVertexStride = componentStride;

    componentOffset += GLOOST_MESH_NUM_COMPONENTS_VERTEX;
    componentStride += GLOOST_MESH_SIZEOF_VERTEX;
  }

  if (isInterleavedAttributeSupported(GLOOST_MESH_NORMALS))
  {
    _interleavedInfo.interleavedNormalOffset = componentOffset;
    _interleavedInfo.interleavedNormalStride = componentStride;

    componentOffset += GLOOST_MESH_NUM_COMPONENTS_NORMAL;
    componentStride += GLOOST_MESH_SIZEOF_NORMAL;
  }

  if (isInterleavedAttributeSupported(GLOOST_MESH_COLORS))
  {
    _interleavedInfo.interleavedColorOffset = componentOffset;
    _interleavedInfo.interleavedColorStride = componentStride;

    componentOffset += GLOOST_MESH_NUM_COMPONENTS_COLOR;
    componentStride += GLOOST_MESH_SIZEOF_COLOR;
  }

  if (isInterleavedAttributeSupported(GLOOST_MESH_TEXCOORDS))
  {
    _interleavedInfo.interleavedTexcoordOffset = componentOffset;
    _interleavedInfo.interleavedTexcoordStride = componentStride;

    componentOffset += GLOOST_MESH_NUM_COMPONENTS_TEXCOORD;
    componentStride += GLOOST_MESH_SIZEOF_TEXCOORD;
  }

  _interleavedInfo.interleavedPackageSize   = componentOffset;
  _interleavedInfo.interleavedPackageStride = componentStride;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief prints the meshInfo
  \remarks ...
*/

void
Mesh::printMeshInfo() const
{
  std::cerr << std::endl << "Message from Mesh::printMeshInfo(): ";
  std::cerr << std::endl;
  std::cerr << std::endl << "             Attributes:";
  std::cerr << std::endl << "               Positions: ............ " << _positions.size();
  std::cerr << std::endl << "               Normals: .............. " << _normals.size();
  std::cerr << std::endl << "               Colors: ............... " << _colors.size();
  std::cerr << std::endl << "               Texcoords: ............ " << _texCoords.size();
  std::cerr << std::endl << "               Interleaved Attributes: " << _interleavedAttributes.size();
  std::cerr << std::endl;
  std::cerr << std::endl << "             Primitives:";
  std::cerr << std::endl << "               Points: ..... " << _points.size();
  std::cerr << std::endl << "               Lines: ...... " << _lines.size();
  std::cerr << std::endl << "               Triangles: .. " << _triangles.size();
  std::cerr << std::endl << "               Quads: ...... " << _quads.size();
  std::cerr << std::endl << "               Material ids: " << _materialIds.size();
  std::cerr << std::endl;
  std::cerr << std::endl << "             MeshInfo:";
  std::cerr << std::endl << "               number of interleaved Packages: " << getNumInterleavedPackages();
  std::cerr << std::endl;
  std::cerr << std::endl << "               interleavedVertexOffset:   " << _interleavedInfo.interleavedVertexOffset;
  std::cerr << std::endl << "               interleavedNormalOffset:   " << _interleavedInfo.interleavedNormalOffset;
  std::cerr << std::endl << "               interleavedColorOffset:    " << _interleavedInfo.interleavedColorOffset;
  std::cerr << std::endl << "               interleavedTexcoordOffset: " << _interleavedInfo.interleavedTexcoordOffset;
  std::cerr << std::endl << "               interleavedPackageSize:    " << _interleavedInfo.interleavedPackageSize;
  std::cerr << std::endl;
  std::cerr << std::endl << "               interleavedVertexStride:   " << _interleavedInfo.interleavedVertexStride;
  std::cerr << std::endl << "               interleavedNormalStride:   " << _interleavedInfo.interleavedNormalStride;
  std::cerr << std::endl << "               interleavedColorStride:    " << _interleavedInfo.interleavedColorStride;
  std::cerr << std::endl << "               interleavedTexcoordStride: " << _interleavedInfo.interleavedTexcoordStride;
  std::cerr << std::endl << "               interleavedPackageStride:  " << _interleavedInfo.interleavedPackageStride;
  std::cerr << std::endl;
  std::cerr << std::endl << "             Other attributes:";
  std::cerr << std::endl << "               Bounding box min: " << _boundingBox.getPMin();
  std::cerr << std::endl << "               Bounding box max: " << _boundingBox.getPMax();
  std::cerr << std::endl;
}

////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


