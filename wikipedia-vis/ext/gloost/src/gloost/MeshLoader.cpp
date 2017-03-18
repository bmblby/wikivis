
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
#include <gloost/MeshLoader.h>
#include <gloost/Mesh.h>



// cpp includes


namespace gloost
{

/**
  \class MeshLoader

  \brief Base class of all mesh loaders

  \author  Felix Weiszig
  \date    March 2011
  \remarks ...
*/

///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

MeshLoader::MeshLoader(const std::string& filePath):
  MeshLoader(gloost::PathType(filePath))
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

MeshLoader::MeshLoader(const gloost::PathType& filePath):
  _filePath(filePath),
  _mesh(Mesh::create())
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

/*virtual*/
MeshLoader::~MeshLoader()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns a Geometry object, filled with the loaded geometry
*/

/*virtual*/
std::shared_ptr<Mesh>
MeshLoader::getMesh()
{
  return _mesh;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the file path of the mesh file
*/

/*virtual*/
const std::string
MeshLoader::getFilePathString() const
{
  return _filePath.string();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the file path of the mesh file
*/

/*virtual*/
const gloost::PathType&
MeshLoader::getFilePath() const
{
  return _filePath;
}


///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////


} // namespace gloost

