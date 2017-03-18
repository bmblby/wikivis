
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
#include <gloost/GbmLoader.h>
#include <gloost/Mesh.h>
#include <gloost/BinaryFile.h>



// cpp includes
//#include <string>
//#include <iostream>
//#include <vector>


namespace gloost
{

/**
  \class GbmLoader

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

GbmLoader::GbmLoader(const gloost::PathType& filePath):
  MeshLoader(filePath)
{
  readFile(filePath);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

/*virtual*/
GbmLoader::~GbmLoader()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief read the file
  \param ...
  \remarks ...
*/

/*virtual*/
bool
GbmLoader::readFile(const gloost::PathType& filePath)
{
  BinaryFile inFile;

  // load the complete file
  if (!inFile.openAndLoad(filePath))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cerr << std::endl << "ERROR in GbmLoader::readFile(const gloost::PathType& filePath): ";
    std::cerr << std::endl << "         While reading \"" << filePath << "\"...";
    std::cerr << std::endl << "         Could NOT open the file.";
    std::cerr << std::endl;
#endif
    return false;
  }

  // check for ply keyword
  std::string word = inFile.readWord();
  if (strcmp(word.c_str(), "GBM"))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cerr << std::endl << "ERROR in GbmLoader::readFile(const gloost::PathType& filePath): ";
    std::cerr << std::endl << "         While reading \"" << filePath << "\"...";
    std::cerr << std::endl << "         File is NOT a gbm file (gloost Binary Mesh)!";
    std::cerr << std::endl;
#endif

    inFile.unload();
    return false;
  }

//#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
//    std::cerr << std::endl << "Message from GbmLoader::readFile(const gloost::PathType& filePath): ";
//    std::cerr << std::endl << "             Parsing \"" << filePath << "\".";
//#endif


  std::string versionString = inFile.readWord();
//  std::cerr << std::endl << "versionString: " << "\"" << versionString << "\".";

  std::string numFeaturesString = inFile.readWord();
//  std::cerr << std::endl << "numCompString: " << "\"" << numFeaturesString << "\".";

  unsigned int numFeatures = atoi(inFile.readWord().c_str());
//  std::cerr << std::endl << "numComponents: " << numFeatures;


  // read list of features and the number of elements for each
  std::vector<GbmFeature> features(numFeatures);

  for (unsigned int i=0; i!=numFeatures; ++i)
  {
    features[i].name  = inFile.readWord();
    features[i].count = atoi(inFile.readWord().c_str());


//#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
//    std::cerr << std::endl << "               " + features[i].name << ": " << features[i].count;
//#endif

    if (!strcmp(features[i].name.c_str(), "interleaved:"))
    {
      features[i].count2 = atoi(inFile.readWord().c_str());

      for (unsigned int f=0; f!=features[i].count; ++f)
      {
        std::string ifeature = inFile.readWord();

//#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
//        std::cerr << std::endl << "                  ifeature: " << ifeature;
//#endif

        if (!strcmp(ifeature.c_str(), "ipositions"))
        {
          _mesh->setSupportedInterleavedAttribute(GLOOST_MESH_POSITIONS, true);
        }
        if (!strcmp(ifeature.c_str(), "inormals"))
        {
          _mesh->setSupportedInterleavedAttribute(GLOOST_MESH_NORMALS, true);
        }
        if (!strcmp(ifeature.c_str(), "icolors"))
        {
          _mesh->setSupportedInterleavedAttribute(GLOOST_MESH_COLORS, true);
        }
        if (!strcmp(ifeature.c_str(), "itexcoords"))
        {
          _mesh->setSupportedInterleavedAttribute(GLOOST_MESH_TEXCOORDS, true);
        }

      }
    }
  }

  // reading data for each feature
  for (unsigned int i=0; i!=numFeatures; ++i)
  {

    // positions
    if (!strcmp(features[i].name.c_str(), "positions:"))
    {
      _mesh->getPositions() = std::vector<Point3>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getPositions()[d][0] = inFile.readFloat32();
        _mesh->getPositions()[d][1] = inFile.readFloat32();
        _mesh->getPositions()[d][2] = inFile.readFloat32();
      }
    }
    // normals
    else if (!strcmp(features[i].name.c_str(), "normals:"))
    {
      _mesh->getNormals() = std::vector<Vector3>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getNormals()[d][0] = inFile.readFloat32();
        _mesh->getNormals()[d][1] = inFile.readFloat32();
        _mesh->getNormals()[d][2] = inFile.readFloat32();
      }
    }
    // colors
    else if (!strcmp(features[i].name.c_str(), "colors:"))
    {
      _mesh->getColors() = std::vector<vec4>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getColors()[d].r = inFile.readFloat32();
        _mesh->getColors()[d].g = inFile.readFloat32();
        _mesh->getColors()[d].b = inFile.readFloat32();
        _mesh->getColors()[d].a = inFile.readFloat32();
      }
    }
    // texCoords
    else if (!strcmp(features[i].name.c_str(), "texcoords:"))
    {
      _mesh->getTexCoords() = std::vector<Point3>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getTexCoords()[d][0] = inFile.readFloat32();
        _mesh->getTexCoords()[d][1] = inFile.readFloat32();
        _mesh->getTexCoords()[d][2] = inFile.readFloat32();
      }
    }
    // interleaved
    else if (!strcmp(features[i].name.c_str(), "interleaved:"))
    {
      _mesh->getInterleavedAttributes() = std::vector<float>(features[i].count);

      for (unsigned int d=0; d!=features[i].count2; ++d)
      {
        _mesh->getInterleavedAttributes()[d] = inFile.readFloat32();
      }
    }
    // points
    else if (!strcmp(features[i].name.c_str(), "points:"))
    {
      _mesh->getPoints() = std::vector<unsigned>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getPoints()[d] = inFile.readInt32();
      }
    }
    // lines
    else if (!strcmp(features[i].name.c_str(), "lines:"))
    {
      _mesh->getLines() = std::vector<Mesh::LineIndices>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getLines()[d].vertexIndices[0] = inFile.readFloat32();
        _mesh->getLines()[d].vertexIndices[1] = inFile.readFloat32();
      }
    }
    // triangles
    else if (!strcmp(features[i].name.c_str(), "triangles:"))
    {
      _mesh->getTriangles() = std::vector<Mesh::TriangleIndices>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getTriangles()[d].vertexIndices[0] = inFile.readFloat32();
        _mesh->getTriangles()[d].vertexIndices[1] = inFile.readFloat32();
        _mesh->getTriangles()[d].vertexIndices[2] = inFile.readFloat32();
      }
    }
    // quads
    else if (!strcmp(features[i].name.c_str(), "quads:"))
    {
      _mesh->getQuads() = std::vector<Mesh::QuadIndices>(features[i].count);

      for (unsigned int d=0; d!=features[i].count; ++d)
      {
        _mesh->getQuads()[d].vertexIndices[0] = inFile.readFloat32();
        _mesh->getQuads()[d].vertexIndices[1] = inFile.readFloat32();
        _mesh->getQuads()[d].vertexIndices[2] = inFile.readFloat32();
        _mesh->getQuads()[d].vertexIndices[3] = inFile.readFloat32();
      }
    }

  }

  _mesh->recalcBoundingBox();

  inFile.unload();
  return true;

}


///////////////////////////////////////////////////////////////////////////////


} // namespace gloost

