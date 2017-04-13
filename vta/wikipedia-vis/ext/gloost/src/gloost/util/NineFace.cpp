
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
#include <gloost/util/NineFace.h>
#include <gloost/Mesh.h>
#include <gloost/InterleavedAttributes.h>
#include <gloost/TextureManager.h>
#include <gloost/MeshFactory.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace util
{


/**
  \class   NineFace

  \brief   creates and stores a parameters and a generated mesh for a nineface

  \author  Felix Weiszig, dynardo gmbh
  \date    November 2013
  \remarks

*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

NineFace::NineFace():
  _textureWidth(100),
	_outerSize(100.f, 100.f, 1.f),
	_left(10.f),
	_right(10.f),
	_bottom(10.f),
	_top(10.f),
	_alignVertexPosToInt(true),
	_textureGid(0),
  _meshHasChanged(true),
  _mesh(nullptr),
  _vertexAttributes(nullptr)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
NineFace::shared_ptr
NineFace::create(unsigned textureWidth,
                 float left, float right,
                 float bottom, float top,
                 bool alignVertexPosToInt)
{

	auto nineFace = new NineFace();

	nineFace->_textureWidth  = textureWidth;
//	nineFace->_outerSize     = Vector3(innerWidth+left+right, innerHeight+bottom+top, 1.0f);
	nineFace->_left          = left;
	nineFace->_right         = right;
	nineFace->_bottom        = bottom;
	nineFace->_top           = top;
	nineFace->_alignVertexPosToInt = alignVertexPosToInt;

	return shared_ptr(nineFace);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
NineFace::~NineFace()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the inner size of the nineface
  \param   ...
  \remarks ...
*/

Vector3
NineFace::getInnerSize() const
{
	return _outerSize - Vector3(_left+_right, _bottom+_top, 0.0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the the inner size of the nineface
  \param   ...
  \remarks ...
*/

void
NineFace::setInnerSize(const Vector3& innerSize)
{
  _outerSize = Vector3(innerSize[0]+_left+_right, innerSize[1]+_bottom+_top, 1.0f);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the outer size of the nineface
  \param   ...
  \remarks ...
*/

const Vector3&
NineFace::getOuterSize() const
{
	return _outerSize;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the the outer size of the nineface
  \param   ...
  \remarks ...
*/

void
NineFace::setOuterSize(const Vector3& size)
{
  _outerSize = size;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the the outer size of the nineface
  \param   ...
  \remarks ...
*/

std::vector<float>
NineFace::getLRBT() const
{
	return {_left, _right, _bottom, _top};
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns true if the mesh is not up to date
  \param   ...
  \remarks ...
*/

bool
NineFace::isMeshDirty() const
{
	return _meshHasChanged;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets a flag indicating that the mesh has changed and therefore must be regenerated
  \param   ...
  \remarks ...
*/

void
NineFace::setMeshDirty(bool flag)
{
	_meshHasChanged = flag;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the gloost::Mesh of the nineFace
  \param   ...
  \remarks ...
*/

std::shared_ptr<Mesh>
NineFace::getMesh()
{
  if (isMeshDirty())
  {
    generate();
  }
	return _mesh;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the gloost::Mesh of the nineFace
  \param   ...
  \remarks ...
*/

//std::shared_ptr<InterleavedAttributes>
//NineFace::getVertexAttribs()
//{
//  if (isMeshDirty())
//  {
//    getMesh();
//  }
//
//	return _vertexAttributes;
//}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the gloost::Mesh of the nineFace
  \param   ...
  \remarks ...
*/

void
NineFace::generate()
{

  auto newMesh = MeshFactory::createNineFace(_textureWidth,
                                             _outerSize[0], _outerSize[1],
                                             _left, _right,
                                             _bottom, _top,
                                             _alignVertexPosToInt);

  newMesh->recalcBoundingBox();

//  auto newVertexAttribs = gloost::InterleavedAttributes::create(newMesh);

  _mesh = newMesh;
//  _vertexAttributes = newVertexAttribs;
  setMeshDirty(false);
}


////////////////////////////////////////////////////////////////////////////////





} // namespace util
} // namespace gloost


