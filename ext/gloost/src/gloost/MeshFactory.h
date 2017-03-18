
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

#ifndef H_GLOOST_MESH_FACTORY
#define H_GLOOST_MESH_FACTORY

// gloost includes
#include <gloost/Mesh.h>
#include <gloost/Point3.h>
#include <gloost/Vector3.h>

// cpp includes
#include <memory.h>

///////////////////////////////////////////////////////////////////////////////

namespace gloost
{
namespace MeshFactory
{

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a Quad located between (0.0,0.0,0.0) and (1.0,1.0,1.0) build with two triangles
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createTriangleQuad(float sMin=0.0, float sMax=1.0,
                   float tMin=0.0, float tMax=1.0,
                   const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  std::shared_ptr<gloost::Mesh> mesh = Mesh::create();

  // vertices (clockwise)
  mesh->getPositions().push_back(Point3(0.0, 0.0, 0.0));
  mesh->getPositions().push_back(Point3(1.0, 0.0, 0.0));
  mesh->getPositions().push_back(Point3(0.0, 1.0, 0.0));

  mesh->getPositions().push_back(Point3(1.0, 0.0, 0.0));
  mesh->getPositions().push_back(Point3(1.0, 1.0, 0.0));
  mesh->getPositions().push_back(Point3(0.0, 1.0, 0.0));

  // normals
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));

  // colors
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);

  // texcoords
  mesh->getTexCoords().push_back(Point3(sMin, tMin, 0.0));
  mesh->getTexCoords().push_back(Point3(sMax, tMin, 0.0));
  mesh->getTexCoords().push_back(Point3(sMin, tMax, 0.0));

  mesh->getTexCoords().push_back(Point3(sMax, tMin, 0.0));
  mesh->getTexCoords().push_back(Point3(sMax, tMax, 0.0));
  mesh->getTexCoords().push_back(Point3(sMin, tMax, 0.0));

  // indices
  mesh->getTriangles().push_back(Mesh::TriangleIndices(0,1,2));
  mesh->getTriangles().push_back(Mesh::TriangleIndices(3,4,5));

  return mesh;
}

//////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a Quad located between (-0.5,-0.5,-0.5) and (0.5,0.5,0.5) build with two triangles
  \remarks Can be drawn as GL_TRIANGLES and GL_TRIANGLE_STRIP
*/

inline
gloost::Mesh::shared_ptr
createTriangleQuadCenter(float sMin=0.0, float sMax=1.0,
                         float tMin=0.0, float tMax=1.0,
                         const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  std::shared_ptr<Mesh> mesh = createTriangleQuad(sMin, sMax, tMin, tMax, color);

  // vertices
  mesh->getPositions()[0] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[1] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[2] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[3] -= gloost::Vector3(0.5, 0.5, 0.0);

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a Quad located between (0.0,0.0,0.0) and (1.0,1.0,1.0)
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createQuad(float sMin=0.0, float sMax=1.0,
           float tMin=0.0, float tMax=1.0,
           const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();

  // vertices
  mesh->getPositions().push_back(Point3(0.0, 0.0, 0.0));
  mesh->getPositions().push_back(Point3(0.0, 1.0, 0.0));
  mesh->getPositions().push_back(Point3(1.0, 1.0, 0.0));
  mesh->getPositions().push_back(Point3(1.0, 0.0, 0.0));

  // normals
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));

  // colors
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);

  // texcoords
  mesh->getTexCoords().push_back(Point3(sMin, tMin, 0.0));
  mesh->getTexCoords().push_back(Point3(sMin, tMax, 0.0));
  mesh->getTexCoords().push_back(Point3(sMax, tMax, 0.0));
  mesh->getTexCoords().push_back(Point3(sMax, tMin, 0.0));

  // indices
  mesh->getQuads().push_back(Mesh::QuadIndices(0,1,2,3));

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

//
///**
//  \brief   creates a triangulated Quad located between (0.0,0.0,0.0) and (1.0,1.0,1.0)
//  \remarks ...
//*/
//
//inline
//gloost::Mesh::shared_ptr
//createTriangleQuad(float sMin=0.0, float sMax=1.0,
//                   float tMin=0.0, float tMax=1.0,
//                   const vec4& color = vec4(1.0,1.0,1.0,1.0))
//{
//  auto quadMesh = gloost::Mesh::create();
//
//  // positions
//  auto& quadPoints = quadMesh->getPositions();
//  quadPoints.resize(4);
//
//  quadPoints[0] = gloost::Point3(0.0,0.0,0.0);
//  quadPoints[1] = gloost::Point3(1.0,0.0,0.0);
//  quadPoints[2] = gloost::Point3(1.0,1.0,0.0);
//  quadPoints[3] = gloost::Point3(0.0,1.0,0.0);
//
//  // colors
//  auto& quadColors = quadMesh->getColors();
//  quadColors.resize(4);
//
//  quadColors[0] = gloost::vec4(1.0,0.0,0.0,1.0);
//  quadColors[1] = gloost::vec4(0.0,1.0,0.0,1.0);
//  quadColors[2] = gloost::vec4(0.0,0.0,1.0,1.0);
//  quadColors[3] = gloost::vec4(1.0,1.0,0.0,1.0);
//
//  // indices
//  auto& triangleIndices = quadMesh->getTriangles();
//  triangleIndices.resize(2);
//
//  triangleIndices[0] = {0,1,2};
//  triangleIndices[1] = {0,2,3};
//
//  // normals
//  auto& normals = quadMesh->getNormals();
//  normals.resize(4);
//
//  normals[0] = {0.0,0.0,1.0};
//  normals[1] = {0.0,0.0,1.0};
//  normals[2] = {0.0,0.0,1.0};
//  normals[3] = {0.0,0.0,1.0};
//
//  auto& texCoords = quadMesh->getTexCoords();
//  texCoords.resize(4);
//
//  texCoords[0] = {0.0,0.0,0.0};
//  texCoords[1] = {1.0,0.0,0.0};
//  texCoords[2] = {1.0,1.0,0.0};
//  texCoords[3] = {0.0,1.0,0.0};
//
//  return mesh;
//}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a Quad located between (-0.5,-0.5,-0.5) and (0.5,0.5,0.5)
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createQuadCenter(float sMin=0.0, float sMax=1.0,
                 float tMin=0.0, float tMax=1.0,
                 const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = createQuad(sMin, sMax, tMin, tMax, color);

  // vertices
  mesh->getPositions()[0] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[1] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[2] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[3] -= gloost::Vector3(0.5, 0.5, 0.0);

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a Quad located between (0.0,0.0,0.0) and (1.0,1.0,1.0) build with four lines
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createLineQuad(const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();

  // vertices
  mesh->getPositions().push_back(Point3(0.0, 0.0, 0.0));
  mesh->getPositions().push_back(Point3(0.0, 1.0, 0.0));
  mesh->getPositions().push_back(Point3(1.0, 1.0, 0.0));
  mesh->getPositions().push_back(Point3(1.0, 0.0, 0.0));
  mesh->getPositions().push_back(Point3(0.0, 0.0, 0.0));

  // normals
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));
  mesh->getNormals().push_back(Vector3(0.0, 0.0, 1.0));

  // colors
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);
  mesh->getColors().push_back(color);

  // indices
  mesh->getLines().push_back(Mesh::LineIndices(0,1));
  mesh->getLines().push_back(Mesh::LineIndices(1,2));
  mesh->getLines().push_back(Mesh::LineIndices(2,3));
  mesh->getLines().push_back(Mesh::LineIndices(3,0));

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a Quad located between (-0.5,-0.5,-0.5) and (0.5,0.5,0.5) build with four lines
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createLineQuadCenter(const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = createLineQuad(color);
  mesh->getPositions()[0] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[1] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[2] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[3] -= gloost::Vector3(0.5, 0.5, 0.0);
  mesh->getPositions()[4] -= gloost::Vector3(0.5, 0.5, 0.0);
  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a circle with numSegments segments build with triangles
  \remarks Can be drawn as GL_TRIANGLE_FAN
*/

inline
gloost::Mesh::shared_ptr
createTriangleCircle(unsigned numSegments = 18u,
                     float    radius      = 0.5f,
                     const    vec4& color = vec4(1.0f,1.0f,1.0f,1.0f))
{
  auto mesh = Mesh::create();
  const float stepWidth = (2.0*math::PI)/(float)numSegments;

  mesh->getPositions().push_back(gloost::Point3(0.0, 0.0, 0.0));
  mesh->getNormals().push_back(gloost::Point3(0.0, 0.0, 1.0));
  mesh->getColors().push_back(color);
  mesh->getTexCoords().push_back(Point3(0.5, 0.5, 0.0));


  // vertices, normals and texcoords
  for (unsigned i=0; i!=numSegments+1; ++i)
  {
    mesh->getPositions().push_back(gloost::Point3(sin(i*stepWidth)*radius, cos(i*stepWidth)*radius, 0.0));
    mesh->getNormals().push_back(gloost::Point3(0.0, 0.0, 1.0));
    mesh->getColors().push_back(color);
    mesh->getTexCoords().push_back(gloost::Point3(sin(i*stepWidth)*0.5+1.0, cos(i*stepWidth)*0.5+1.0, 0.0));
  }

  // indices
  for (int i=1; i!=(int)numSegments+2; ++i)
  {
    mesh->getTriangles().push_back(Mesh::TriangleIndices(0, i-1, i));
  }

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a circle with numSegments segments build with lines
  \remarks Can be drawn as GL_LINE_STRIP
*/

inline
gloost::Mesh::shared_ptr
createLineCircle(unsigned numSegments = 18u,
                 float    radius      = 0.5f,
                 const    vec4& color = vec4(1.0f,1.0f,1.0f,1.0f))
{
  auto mesh = Mesh::create();
  float stepWidth = (2*math::PI)/(float)numSegments;

  // vertices and colors
  for (unsigned i=0; i!=numSegments+1; ++i)
  {
    mesh->getPositions().push_back(gloost::Point3(sin(i*stepWidth)*radius,
                                   cos(i*stepWidth)*radius, 0.0f));
    mesh->getColors().push_back(color);
  }

  // indices
  for (int i=1; i!=(int)numSegments+1; ++i)
  {
    mesh->getLines().push_back(Mesh::LineIndices(i-1, i));
  }

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a point grid between (0.0,0.0,0.0) and (1.0,1.0,1.0)
  \remarks This will creates tex coords (from 0.0 ... 0.5) for each point
*/

inline
gloost::Mesh::shared_ptr
createPointGrid(unsigned numColums,
                unsigned numRows,
                const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();

  float stepH = 1.0f/numColums;
  float stepV = 1.0f/numRows;
  unsigned index = 0;
  for(unsigned int y=0; y != numRows; ++y)
    for(unsigned int x=0; x != numColums; ++x)
    {
      mesh->getPositions().push_back( Point3(stepH*x, stepV*y, 0.0));
      mesh->getTexCoords().push_back( Point3(stepH*x, stepV*y, 0.0));
      mesh->getColors().push_back(color);
      mesh->getPoints().push_back(index++);
    }

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a point grid between (-0.5,-0.5,-0.5) and (0.5,0.5,0.5)
  \remarks This will creates tex coords (from 0.0 ... 0.5) for each point
*/

inline
gloost::Mesh::shared_ptr
createPointGridCenter(unsigned numColums,
                      unsigned numRows,
                      const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  gloost::Mesh::shared_ptr mesh = createPointGrid(numColums, numRows, color);

  for (unsigned i=0; i!=mesh->getPositions().size(); ++i)
  {
    mesh->getPositions()[i] -= gloost::Vector3(0.5, 0.5, 0.0);
  }

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a line grid between (0.0,0.0,0.0) and (1.0,1.0,1.0)
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createLineGrid(unsigned numColums,
               unsigned numRows,
               const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();
  unsigned vertexindex = 0;

  float step = 1.0f/numColums;
  for(unsigned int i=0; i != numColums+1; ++i)
  {
    mesh->getPositions().push_back( Point3(step*i,  1.0, 0.0));
    mesh->getPositions().push_back( Point3(step*i,  0.0, 0.0));

    mesh->getColors().push_back(color);
    mesh->getColors().push_back(color);

    mesh->getLines().push_back(Mesh::LineIndices(vertexindex, vertexindex+1));
    vertexindex += 2;
  }

  step = 1.0f/numRows;
  for(unsigned int i=0; i != numRows+1; ++i)
  {
    mesh->getPositions().push_back( Point3(1.0, step*i, 0.0));
    mesh->getPositions().push_back( Point3(0.0, step*i, 0.0));

    mesh->getColors().push_back(color);
    mesh->getColors().push_back(color);

    mesh->getLines().push_back(Mesh::LineIndices(vertexindex, vertexindex+1));
    vertexindex += 2;
  }

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a line grid between (0.5,0.5,0.5) and (0.5,0.5,0.5)
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createLineGridCenter(unsigned int numColums,
                     unsigned numRows,
                     const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  gloost::Mesh::shared_ptr mesh = createLineGrid(numColums, numRows, color);

  for (unsigned i=0; i!=mesh->getPositions().size(); ++i)
  {
    mesh->getPositions()[i] -= gloost::Vector3(0.5, 0.5, 0.0);
  }

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an arrow, pointing up, build with lines
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createTriangleArrow(const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();
  mesh->getPositions().push_back(gloost::Point3( 0.0,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.5, -0.2,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.5,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.0,  0.2,  0.0f));
  mesh->getPositions().push_back(gloost::Point3(-0.5,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3(-0.5, -0.2,  0.0f));

  // normals and colors and texcoords
  for (unsigned i=0; i!=6; ++i)
  {
    mesh->getNormals().push_back(gloost::Vector3(0.0,0.0,1.0));
    mesh->getColors().push_back(color);
    mesh->getTexCoords().push_back(mesh->getPositions()[i] + gloost::Vector3(0.5, 0.5, 0.0));
  }

  for (unsigned i=0; i!=mesh->getPositions().size()-1; ++i)
  {
    mesh->getTriangles().push_back(Mesh::TriangleIndices(0, i, i+1));
  }
  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an arrow, pointing up, build with lines
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createTriangleArrow2(const gloost::vec4& color)
{
  auto mesh = Mesh::create();
  mesh->getPositions().push_back(gloost::Point3(  2.0, 0.0, 0.0f));  // 0
  mesh->getPositions().push_back(gloost::Point3( -2.0, 0.0, 0.0f));  // 1
  mesh->getPositions().push_back(gloost::Point3(  2.0, 9.0, 0.0f));  // 2
  mesh->getPositions().push_back(gloost::Point3( -2.0, 9.0, 0.0f));  // 3
  mesh->getPositions().push_back(gloost::Point3( -5.0, 7.0, 0.0f));  // 4
  mesh->getPositions().push_back(gloost::Point3( -5.0, 10.0, 0.0f)); // 5
  mesh->getPositions().push_back(gloost::Point3(  0.0, 14.0, 0.0f)); // 6
  mesh->getPositions().push_back(gloost::Point3(  5.0, 10.0, 0.0f)); // 7
  mesh->getPositions().push_back(gloost::Point3(  5.0, 7.0,  0.0f)); // 8

  mesh->recalcBoundingBox();
  mesh->scaleToSize(1.0f);

  auto& trianglesIndiceVector = mesh->getTriangles();
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(0,1,2));  // A
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(1,3,2));  // B
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(4,5,3));  // C
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(3,5,6));  // D
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(2,3,6));  // E
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(7,2,6));  // F
  trianglesIndiceVector.push_back(Mesh::TriangleIndices(8,2,7));  // G

  // normals and colors and texcoords
  for (unsigned i=0; i!=mesh->getPositions().size(); ++i)
  {
    mesh->getNormals().push_back(gloost::Vector3(0.0,0.0,1.0));
    mesh->getColors().push_back(color);
    mesh->getTexCoords().push_back(mesh->getPositions()[i] + gloost::Vector3(0.5, 0.5, 0.0));
  }

  mesh->recalcBoundingBox();

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an arrow, pointing up, build with lines
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createLineArrow(const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();
  mesh->getPositions().push_back(gloost::Point3( 0.1, -0.8,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.1,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.3, -0.2,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.3,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3( 0.0,  0.4,  0.0f));
  mesh->getPositions().push_back(gloost::Point3(-0.3,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3(-0.3, -0.2,  0.0f));
  mesh->getPositions().push_back(gloost::Point3(-0.1,  0.0,  0.0f));
  mesh->getPositions().push_back(gloost::Point3(-0.1, -0.8,  0.0f));

  // normals and colors
  for (unsigned i=0; i!=10; ++i)
  {
    mesh->getColors().push_back(color);
  }

  for (unsigned i=0; i!=mesh->getPositions().size()-1; ++i)
  {
    mesh->getLines().push_back(Mesh::LineIndices(i, i+1));
  }
  mesh->getLines().push_back(Mesh::LineIndices(9, 0));
  return mesh;
}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a square grid build from a single triangle strip facing +z
  \remarks Mesh should be drawn as GL_TRIANGLE_STRIP
  \remarks Original code taken from
           http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
*/

inline
gloost::Mesh::shared_ptr
createTriangleStripGrid(unsigned resH,
                        unsigned resV,
                        const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();


  // vertex positions
  const float stepX = 1.0f/resH;
  const float stepY = 1.0f/resV;

  Point3 vertexPos(0.0, 0.0, 0.0);

  mesh->getPositions().resize(resH*resV);
  int i = 0;

  for (unsigned row=0; row!=resV; ++row )
  {
    for (unsigned col=0; col!=resH; ++col)
    {
      vertexPos[0] = col*stepX;
      vertexPos[1] = row*stepY;
      mesh->getPositions()[i++] = vertexPos;
    }
  }

  // indices
  mesh->getPoints().resize((resH*resV) + (resH-1)*(resV-2));

  i = 0;

  for (unsigned row=0; row<resV-1; ++row )
  {
    if ( (row&1)==0 )   // even rows
    {
      for ( int col=0; col< (int)resH; ++col)
      {
        mesh->getPoints()[i++] = col + row * resH;
        mesh->getPoints()[i++] = col + (row+1) * resH;
      }
    }
    else     // odd rows
    {
      for (int col=resH-1; col>0; --col )
      {
        mesh->getPoints()[i++] = col + (row+1) * resH;
        mesh->getPoints()[i++] = col - 1 + row * resH;
      }
    }
  }

  if ( (resV & 1) && resV > 2 ) {
        mesh->getPoints()[i++] = (resV-1) * resH;
    }


  // color
  mesh->getColors().resize(mesh->getPositions().size(), color);

  mesh->recalcBoundingBox();
  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a box from (0.0,0.0,0.0) to (sideLength,sideLength,sideLength)
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createQuadBox(float sideLength = 1.0f, const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();

  const gloost::Point3 A = gloost::Point3(-0.5, -0.5, 0.5) * (mathType)sideLength;
  const gloost::Point3 B = gloost::Point3(0.5, -0.5, 0.5)  * (mathType)sideLength;
  const gloost::Point3 C = gloost::Point3(-0.5, -0.5, -0.5)* (mathType)sideLength;
  const gloost::Point3 D = gloost::Point3(0.5, -0.5, -0.5) * (mathType)sideLength;
  const gloost::Point3 E = gloost::Point3(-0.5, 0.5, 0.5)  * (mathType)sideLength;
  const gloost::Point3 F = gloost::Point3(0.5, 0.5, 0.5)   * (mathType)sideLength;
  const gloost::Point3 G = gloost::Point3(-0.5, 0.5, -0.5) * (mathType)sideLength;
  const gloost::Point3 H = gloost::Point3(0.5, 0.5, -0.5)  * (mathType)sideLength;

  mesh->getPositions() = std::vector<Point3>(24);
  mesh->getNormals()  = std::vector<Vector3>(24);
  mesh->getColors()   = std::vector<vec4>(24);


  gloost::Vector3 normal;

//  int index = 0;

// top: EGF + GHF
  normal = gloost::Vector3(0.0, 1.0, 0.0);

  mesh->getPositions()[0] = (E);
  mesh->getNormals()[0]  = (normal);
  mesh->getColors()[0]   = (color);

  mesh->getPositions()[1] = (G);
  mesh->getNormals()[1]  = (normal);
  mesh->getColors()[1]   = (color);

  mesh->getPositions()[2] = (F);
  mesh->getNormals()[2]  = (normal);
  mesh->getColors()[2]   = (color);

  mesh->getPositions()[3] = (H);
  mesh->getNormals()[3]  = (normal);
  mesh->getColors()[3]   = (color);

//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(3, 2, 0, 1));

// left: CGA + GEA
  normal = gloost::Vector3(-1.0, 0.0, 0.0);

  mesh->getPositions()[4] = (C);
  mesh->getNormals()[4] = (normal);
  mesh->getColors()[4] = (color);

  mesh->getPositions()[5] = (G);
  mesh->getNormals()[5] = (normal);
  mesh->getColors()[5] = (color);

  mesh->getPositions()[6] = (A);
  mesh->getNormals()[6] = (normal);
  mesh->getColors()[6] = (color);

  mesh->getPositions()[7] = (E);
  mesh->getNormals()[7] = (normal);
  mesh->getColors()[7] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(7, 6, 4, 5));


// front: AEB + EFB
  normal = gloost::Vector3(0.0, 0.0, 1.0);

  mesh->getPositions()[8] = (A);
  mesh->getNormals()[8] = (normal);
  mesh->getColors()[8] = (color);

  mesh->getPositions()[9] = (E);
  mesh->getNormals()[9] = (normal);
  mesh->getColors()[9] = (color);

  mesh->getPositions()[10] = (B);
  mesh->getNormals()[10] = (normal);
  mesh->getColors()[10] = (color);

  mesh->getPositions()[11] = (F);
  mesh->getNormals()[11] = (normal);
  mesh->getColors()[11] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(11, 10, 8, 9));


// right: BFD + FHD
  normal = gloost::Vector3(1.0, 0.0, 0.0);

  mesh->getPositions()[12] = (B);
  mesh->getNormals()[12] = (normal);
  mesh->getColors()[12] = (color);

  mesh->getPositions()[13] = (F);
  mesh->getNormals()[13] = (normal);
  mesh->getColors()[13] = (color);

  mesh->getPositions()[14] = (D);
  mesh->getNormals()[14] = (normal);
  mesh->getColors()[14] = (color);

  mesh->getPositions()[15] = (H);
  mesh->getNormals()[15] = (normal);
  mesh->getColors()[15] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(15, 14, 12, 13));

// bottom: CAD + ABD
  normal = gloost::Vector3(0.0, -1.0, 0.0);

  mesh->getPositions()[16] = (C);
  mesh->getNormals()[16] = (normal);
  mesh->getColors()[16] = (color);


  mesh->getPositions()[17] = (A);
  mesh->getNormals()[17] = (normal);
  mesh->getColors()[17] = (color);

  mesh->getPositions()[18] = (D);
  mesh->getNormals()[18] = (normal);
  mesh->getColors()[18] = (color);

  mesh->getPositions()[19] = (B);
  mesh->getNormals()[19] = (normal);
  mesh->getColors()[19] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(19, 18, 16, 17));

// back: GCH + CDH
  normal = gloost::Vector3(0.0, 0.0, -1.0);

  mesh->getPositions()[20] = (G);
  mesh->getNormals()[20] = (normal);
  mesh->getColors()[20] = (color);

  mesh->getPositions()[21] = (C);
  mesh->getNormals()[21] = (normal);
  mesh->getColors()[21] = (color);

  mesh->getPositions()[22] = (H);
  mesh->getNormals()[22] = (normal);
  mesh->getColors()[22] = (color);

  mesh->getPositions()[23] = (D);
  mesh->getNormals()[23] = (normal);
  mesh->getColors()[23] = (color);

//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(23, 22, 20, 21));

  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a box from (0.0,0.0,0.0) to (sideLength,sideLength,sideLength)
  \remarks ...
*/

inline
gloost::Mesh::shared_ptr
createQuadBox(Point3 pMin = gloost::Point3(-0.5,-0.5,-0.5),
              Point3 pMax = gloost::Point3(0.5,0.5,0.5),
              const vec4& color = vec4(1.0,1.0,1.0,1.0))
{
  auto mesh = Mesh::create();

  const gloost::Point3 A = gloost::Point3(pMin[0], pMin[1], pMax[2]);
  const gloost::Point3 B = gloost::Point3(pMax[0], pMin[1], pMax[2]);
  const gloost::Point3 C = gloost::Point3(pMin[0], pMin[1], pMin[2]);
  const gloost::Point3 D = gloost::Point3(pMax[0], pMin[1], pMin[2]);
  const gloost::Point3 E = gloost::Point3(pMin[0], pMax[1], pMax[2]);
  const gloost::Point3 F = gloost::Point3(pMax[0], pMax[1], pMax[2]);
  const gloost::Point3 G = gloost::Point3(pMin[0], pMax[1], pMin[2]);
  const gloost::Point3 H = gloost::Point3(pMax[0], pMax[1], pMin[2]);

  mesh->getPositions() = std::vector<Point3>(24);
  mesh->getNormals()   = std::vector<Vector3>(24);
  mesh->getColors()    = std::vector<vec4>(24);


  gloost::Vector3 normal;

//  int index = 0;

// top: EGF + GHF
  normal = gloost::Vector3(0.0, 1.0, 0.0);

  mesh->getPositions()[0] = (E);
  mesh->getNormals()[0]  = (normal);
  mesh->getColors()[0]   = (color);

  mesh->getPositions()[1] = (G);
  mesh->getNormals()[1]  = (normal);
  mesh->getColors()[1]   = (color);

  mesh->getPositions()[2] = (F);
  mesh->getNormals()[2]  = (normal);
  mesh->getColors()[2]   = (color);

  mesh->getPositions()[3] = (H);
  mesh->getNormals()[3]  = (normal);
  mesh->getColors()[3]   = (color);

//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(3, 2, 0, 1));

// left: CGA + GEA
  normal = gloost::Vector3(-1.0, 0.0, 0.0);

  mesh->getPositions()[4] = (C);
  mesh->getNormals()[4] = (normal);
  mesh->getColors()[4] = (color);

  mesh->getPositions()[5] = (G);
  mesh->getNormals()[5] = (normal);
  mesh->getColors()[5] = (color);

  mesh->getPositions()[6] = (A);
  mesh->getNormals()[6] = (normal);
  mesh->getColors()[6] = (color);

  mesh->getPositions()[7] = (E);
  mesh->getNormals()[7] = (normal);
  mesh->getColors()[7] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(7, 6, 4, 5));


// front: AEB + EFB
  normal = gloost::Vector3(0.0, 0.0, 1.0);

  mesh->getPositions()[8] = (A);
  mesh->getNormals()[8] = (normal);
  mesh->getColors()[8] = (color);

  mesh->getPositions()[9] = (E);
  mesh->getNormals()[9] = (normal);
  mesh->getColors()[9] = (color);

  mesh->getPositions()[10] = (B);
  mesh->getNormals()[10] = (normal);
  mesh->getColors()[10] = (color);

  mesh->getPositions()[11] = (F);
  mesh->getNormals()[11] = (normal);
  mesh->getColors()[11] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(11, 10, 8, 9));


// right: BFD + FHD
  normal = gloost::Vector3(1.0, 0.0, 0.0);

  mesh->getPositions()[12] = (B);
  mesh->getNormals()[12] = (normal);
  mesh->getColors()[12] = (color);

  mesh->getPositions()[13] = (F);
  mesh->getNormals()[13] = (normal);
  mesh->getColors()[13] = (color);

  mesh->getPositions()[14] = (D);
  mesh->getNormals()[14] = (normal);
  mesh->getColors()[14] = (color);

  mesh->getPositions()[15] = (H);
  mesh->getNormals()[15] = (normal);
  mesh->getColors()[15] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(15, 14, 12, 13));

// bottom: CAD + ABD
  normal = gloost::Vector3(0.0, -1.0, 0.0);

  mesh->getPositions()[16] = (C);
  mesh->getNormals()[16] = (normal);
  mesh->getColors()[16] = (color);


  mesh->getPositions()[17] = (A);
  mesh->getNormals()[17] = (normal);
  mesh->getColors()[17] = (color);

  mesh->getPositions()[18] = (D);
  mesh->getNormals()[18] = (normal);
  mesh->getColors()[18] = (color);

  mesh->getPositions()[19] = (B);
  mesh->getNormals()[19] = (normal);
  mesh->getColors()[19] = (color);


//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(19, 18, 16, 17));

// back: GCH + CDH
  normal = gloost::Vector3(0.0, 0.0, -1.0);

  mesh->getPositions()[20] = (G);
  mesh->getNormals()[20] = (normal);
  mesh->getColors()[20] = (color);

  mesh->getPositions()[21] = (C);
  mesh->getNormals()[21] = (normal);
  mesh->getColors()[21] = (color);

  mesh->getPositions()[22] = (H);
  mesh->getNormals()[22] = (normal);
  mesh->getColors()[22] = (color);

  mesh->getPositions()[23] = (D);
  mesh->getNormals()[23] = (normal);
  mesh->getColors()[23] = (color);

//    triangles.push_back(gloost::Mesh::TriangleIndices(index, index+1, index+2));
//    triangles.push_back(gloost::Mesh::TriangleIndices(index+1, index+3, index+2));

  mesh->getQuads().push_back(gloost::Mesh::QuadIndices(23, 22, 20, 21));


  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a NineFace with normalized texcoords build out of quads with unshared vertices
  \remarks This is usefull to draw buttons and stuff with rounded edges using a texture. While
           the border defined by left,right,bottom,top (which depends on your
           texture) can stay the same, the fullWidth and fullHeight can vary with the content
           like text... . <br/>
           The resultig mesh uses unshared vertices so no IBO is necessary to draw it. Just use
           ->draw(GL_TRIANGLE);<br/>
           And this is a NineFace:
  \code
               |left|                 |right|

           --  ------------------------------  --
               |    |                 |     |  top
               |----|-----------------|-----|  --
               |    |                 |     |
  fullHeight   |    |                 |     |
               |    |                 |     |
               |    |                 |     |
               |----|-----------------|-----|  --
               |    |                 |     |  bottom
           --  |----|-----------------|-----|  --

               |         fullWidth        |
  \endcode

  \remarks Indexing while contruction
  \code
               |left|                 |right|

           --  m----n-----------------o-----p  --
               | G  |       H         |  I  |  top
               i----j-----------------k-----l  --
               |    |                 |     |
  fullHeight   |    |                 |     |
               | D  |       E         |  F  |
               |    |                 |     |
               e----f-----------------g-----h  --
               | A  |       B         |  C  |  bottom
           --  a----b-----------------c-----d  --

               |         fullWidth        |


  \endcode
*/

inline
gloost::Mesh::shared_ptr
createNineFace(unsigned textureSideLegth,
               float fullWidth, float fullHeight,
               float left, float right,
               float bottom, float top,
               bool alignVertexPosToInt = false)
{

  auto mesh = Mesh::create();

  const double texSideLengthInv = 1.0f/textureSideLegth;

  // from bottom left to top right
  const Point3 a(0.0,0.0,0.0);
  const Point3 b(left,0.0,0.0);
  const Point3 c(fullWidth-right,0.0,0);
  const Point3 d(fullWidth,0.0,0.0);

  const Point3 at = Point3(0.0,0.0,0.0) * texSideLengthInv;
  const Point3 bt = Point3(left,0,0) * texSideLengthInv;
  const Point3 ct = Point3(textureSideLegth-right,0,0) * texSideLengthInv;
  const Point3 dt = Point3(textureSideLegth,0,0) * texSideLengthInv;

  const Point3 e(0,bottom,0);
  const Point3 f(left,bottom,0);
  const Point3 g(fullWidth-right,bottom,0);
  const Point3 h(fullWidth,bottom,0);

  const Point3 et = Point3(0,bottom,0) * texSideLengthInv;
  const Point3 ft = Point3(left,bottom,0) * texSideLengthInv;
  const Point3 gt = Point3(textureSideLegth-right,bottom,0) * texSideLengthInv;
  const Point3 ht = Point3(textureSideLegth,bottom,0) * texSideLengthInv;



  const Point3 i(0,fullHeight-top,0);
  const Point3 j(left,fullHeight-top,0);
  const Point3 k(fullWidth-right,fullHeight-top,0);
  const Point3 l(fullWidth,fullHeight-top,0);

  const Point3 it = Point3(0,textureSideLegth-top,0) * texSideLengthInv;
  const Point3 jt = Point3(left,textureSideLegth-top,0) * texSideLengthInv;
  const Point3 kt = Point3(textureSideLegth-right,textureSideLegth-top,0) * texSideLengthInv;
  const Point3 lt = Point3(textureSideLegth,textureSideLegth-top,0) * texSideLengthInv;


  const Point3 m(0,fullHeight,0);
  const Point3 n(left,fullHeight,0);
  const Point3 o(fullWidth-right,fullHeight,0);
  const Point3 p(fullWidth,fullHeight,0);

  const Point3 mt = Point3(0,textureSideLegth,0) * texSideLengthInv;
  const Point3 nt = Point3(left,textureSideLegth,0) * texSideLengthInv;
  const Point3 ot = Point3(textureSideLegth-right,textureSideLegth,0) * texSideLengthInv;
  const Point3 pt = Point3(textureSideLegth,textureSideLegth,0) * texSideLengthInv;


  std::vector<Point3>& vertices = mesh->getPositions();
  vertices.reserve(36u);
  std::vector<Point3>& texcoords = mesh->getTexCoords();
  texcoords.reserve(36u);

  // quad A
//  vertices.push_back(a); vertices.push_back(e); vertices.push_back(f); vertices.push_back(b);
//  texcoords.push_back(at); texcoords.push_back(et); texcoords.push_back(ft); texcoords.push_back(bt);

  vertices.push_back(a); vertices.push_back(b); vertices.push_back(e);
  texcoords.push_back(at); texcoords.push_back(bt); texcoords.push_back(et);
  vertices.push_back(b); vertices.push_back(f); vertices.push_back(e);
  texcoords.push_back(bt); texcoords.push_back(ft); texcoords.push_back(et);


  // quad B
//  vertices.push_back(b); vertices.push_back(f); vertices.push_back(g); vertices.push_back(c);
//  texcoords.push_back(bt); texcoords.push_back(ft); texcoords.push_back(gt); texcoords.push_back(ct);

  vertices.push_back(b); vertices.push_back(c); vertices.push_back(f);
  texcoords.push_back(bt); texcoords.push_back(ct); texcoords.push_back(ft);
  vertices.push_back(c); vertices.push_back(g); vertices.push_back(f);
  texcoords.push_back(ct); texcoords.push_back(gt); texcoords.push_back(ft);


//  // quad C
//  vertices.push_back(c); vertices.push_back(g); vertices.push_back(h); vertices.push_back(d);
//  texcoords.push_back(ct); texcoords.push_back(gt); texcoords.push_back(ht); texcoords.push_back(dt);

  vertices.push_back(c); vertices.push_back(d); vertices.push_back(g);
  texcoords.push_back(ct); texcoords.push_back(dt); texcoords.push_back(gt);
  vertices.push_back(d); vertices.push_back(h); vertices.push_back(g);
  texcoords.push_back(dt); texcoords.push_back(ht); texcoords.push_back(gt);


//  // quad D
//  vertices.push_back(e);vertices.push_back(i);vertices.push_back(j);vertices.push_back(f);
//  texcoords.push_back(et);texcoords.push_back(it);texcoords.push_back(jt);texcoords.push_back(ft);

  vertices.push_back(e); vertices.push_back(f); vertices.push_back(i);
  texcoords.push_back(et); texcoords.push_back(ft); texcoords.push_back(it);
  vertices.push_back(f); vertices.push_back(j); vertices.push_back(i);
  texcoords.push_back(ft); texcoords.push_back(jt); texcoords.push_back(it);


//  // quad E
//  vertices.push_back(f);vertices.push_back(j);vertices.push_back(k);vertices.push_back(g);
//  texcoords.push_back(ft);texcoords.push_back(jt);texcoords.push_back(kt);texcoords.push_back(gt);

  vertices.push_back(f); vertices.push_back(g); vertices.push_back(j);
  texcoords.push_back(ft); texcoords.push_back(gt); texcoords.push_back(jt);
  vertices.push_back(g); vertices.push_back(k); vertices.push_back(j);
  texcoords.push_back(gt); texcoords.push_back(kt); texcoords.push_back(jt);


//  // quad F
//  vertices.push_back(g);vertices.push_back(k);vertices.push_back(l);vertices.push_back(h);
//  texcoords.push_back(gt);texcoords.push_back(kt);texcoords.push_back(lt);texcoords.push_back(ht);

  vertices.push_back(g); vertices.push_back(h); vertices.push_back(k);
  texcoords.push_back(gt); texcoords.push_back(ht); texcoords.push_back(kt);
  vertices.push_back(h); vertices.push_back(l); vertices.push_back(k);
  texcoords.push_back(ht); texcoords.push_back(lt); texcoords.push_back(kt);


//  // quad G
//  vertices.push_back(i);vertices.push_back(m);vertices.push_back(n);vertices.push_back(j);
//  texcoords.push_back(it);texcoords.push_back(mt);texcoords.push_back(nt);texcoords.push_back(jt);

  vertices.push_back(i); vertices.push_back(j); vertices.push_back(m);
  texcoords.push_back(it); texcoords.push_back(jt); texcoords.push_back(mt);
  vertices.push_back(j); vertices.push_back(n); vertices.push_back(m);
  texcoords.push_back(jt); texcoords.push_back(nt); texcoords.push_back(mt);


//  // quad H
//  vertices.push_back(j);vertices.push_back(n);vertices.push_back(o);vertices.push_back(k);
//  texcoords.push_back(jt);texcoords.push_back(nt);texcoords.push_back(ot);texcoords.push_back(kt);

  vertices.push_back(j); vertices.push_back(k); vertices.push_back(n);
  texcoords.push_back(jt); texcoords.push_back(kt); texcoords.push_back(nt);
  vertices.push_back(k); vertices.push_back(o); vertices.push_back(n);
  texcoords.push_back(kt); texcoords.push_back(ot); texcoords.push_back(nt);


//  // quad I
//  vertices.push_back(k);vertices.push_back(o);vertices.push_back(p);vertices.push_back(l);
//  texcoords.push_back(kt);texcoords.push_back(ot);texcoords.push_back(pt);texcoords.push_back(lt);

  vertices.push_back(k); vertices.push_back(l); vertices.push_back(o);
  texcoords.push_back(kt); texcoords.push_back(lt); texcoords.push_back(ot);
  vertices.push_back(l); vertices.push_back(p); vertices.push_back(o);
  texcoords.push_back(lt); texcoords.push_back(pt); texcoords.push_back(ot);


  if (alignVertexPosToInt)
  {
    for(auto& position : vertices)
    {
      position = position.int_copy();
    }
  }


  return mesh;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace MeshFactory
}  // namespace gloost


#endif // #ifndef H_GLOOST_MESH_FACTORY

