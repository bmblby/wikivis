
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
#include <gloost/PlyLoader.h>
#include <gloost/BinaryFile.h>
#include <gloost/Mesh.h>
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/gloostMath.h>
#include <gloost/gloostHelper.h>
#include <gloost/Material.h>



// cpp includes
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

PlyLoader::PlyLoader(const gloost::PathType& filePath):
  MeshLoader(filePath),
  _inFile(),
  _plyElements(),
  _fileInfo()
{
  readFile(filePath.string());
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

PlyLoader::~PlyLoader()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns a Mesh object, filled with the loaded geometry
*/

std::shared_ptr<Mesh>
PlyLoader::getMesh()
{
  return _mesh;
}


///////////////////////////////////////////////////////////////////////////////


  /// reads a Wavefront Obj file

/*virtual*/
bool
PlyLoader::readFile(const std::string& filePath)
{

  std::vector<Point3>&       vertices       = _mesh->getPositions();
  std::vector<Vector3>&      normals        = _mesh->getNormals();
  std::vector<vec4>&         colors         = _mesh->getColors();
  std::vector<Point3>&       texCoords      = _mesh->getTexCoords();
  std::vector<Mesh::TriangleIndices>& faces = _mesh->getTriangles();
  faces.clear();

  std::string currentMaterialName = GLOOST_OBJMATFILE_DEFAULT_MATERIAL;

  // create a default material
  Material* gloost_default_material = new Material();
  gloost_default_material->set_vec4("v4_ambientColor",  vec4(0.2, 0.2, 0.2, 1.0));
  gloost_default_material->set_vec4("v4_diffuseColor",  vec4(0.6, 0.6, 0.6, 1.0));
  gloost_default_material->set_vec4("v4_specularColor", vec4(1.0, 1.0, 1.0, 1.0));
  gloost_default_material->set_float("v4_specularCoeff", 120.0);

//  _mesh->getMaterials()->addMaterial(currentMaterialName, gloost_default_material);


  // load the complete file
  if (!_inFile.openAndLoad(filePath))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cerr << std::endl << "ERROR in PlyLoader::readFile(const gloost::PathType& filePath): ";
    std::cerr << std::endl << "         While reading \"" << filePath << "\"...";
    std::cerr << std::endl << "         File not found. Maybe your cat ate it.";
    std::cerr << std::endl;
#endif
    return false;
  }

  // check for ply keyword
  std::string word = _inFile.readWord();
  if (strcmp(word.c_str(), "ply"))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cerr << std::endl << "ERROR in PlyLoader::readFile(const gloost::PathType& filePath): ";
    std::cerr << std::endl << "         While reading \"" << filePath << "\"...";
    std::cerr << std::endl << "         File is NOT a ply file (Stanfort Polygon File Format)!";
    std::cerr << std::endl << "         You can use the Free Software \"meshlab\"";
    std::cerr << std::endl << "         (http://meshlab.sourceforge.net/) to convert your files";
    std::cerr << std::endl << "         to ply format.";
    std::cerr << std::endl;
#endif


    _inFile.unload();
    return false;
  }

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
    std::cerr << std::endl << "Message from PlyLoader::readFile(const gloost::PathType& filePath): ";
    std::cerr << std::endl << "             Parsing \"" << filePath << "\".";
#endif

  bool headerDone = false;

  while (!headerDone)
  {
    std::string word = _inFile.readWord();

    // property
    if (strcmp(word.c_str(), "property") == 0)
    {
      readHeaderProperty();
    }

    // element
    else if (strcmp(word.c_str(), "element") == 0)
    {
      readHeaderElement();
    }

    // end_header
    else if (strcmp(word.c_str(), "end_header") == 0)
    {
      break;
    }

    // format
    else if (strcmp(word.c_str(), "format") == 0)
    {
      _fileInfo.formatString = _inFile.readWord();
      _fileInfo.version      = atof(_inFile.readWord().c_str());

      if (strcmp(_fileInfo.formatString.c_str(), "binary_big_endian") == 0)
      {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
        std::cerr << std::endl;
        std::cerr << std::endl << "ERROR in PlyLoader::readFile(): ";
        std::cerr << std::endl << "         While reading \"" << filePath << "\"...";
        std::cerr << std::endl << "         " << _fileInfo.formatString << " is not supported.";
#endif

        return false;
      }

    }

    // comment
    else if (strcmp(word.c_str(), "comment") == 0)
    {
      _fileInfo.comments.push_back(_inFile.readLine());
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
      std::cerr << std::endl << "               comment: " << _fileInfo.comments[_fileInfo.comments.size()-1];
#endif
    }

  }

  // start reading elements
  for (unsigned int e=0; e!=_plyElements.size(); ++e)
  {
    const PlyElement& element = _plyElements[e];

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
    std::cerr << std::endl << "               parsing : " << element.name;
#endif

    if (strcmp(element.name.c_str(), "vertex") == 0)
    {
      vertices = std::vector<Point3>(element.count);
      readVertices(element);
    }
    else if (strcmp(element.name.c_str(), "face") == 0)
    {
//      faces = std::vector<Mesh::TriangleIndices>(element.count);
      readFaces(element);
    }

  }


  _inFile.unload();

  _mesh->recalcBoundingBox();

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
    unsigned int  memoryConsumption =    _mesh->getPositions().size()  * sizeof(Point3)
                                       + _mesh->getNormals().size()   * sizeof(Vector3)
                                       + _mesh->getColors().size()    * sizeof(vec4)
                                       + _mesh->getTexCoords().size() * sizeof(Point3)
                                       + _mesh->getLines().size()     * sizeof(Line);
                                       + _mesh->getTriangles().size() * sizeof(Mesh::TriangleIndices);
                                       + _mesh->getQuads().size()     * sizeof(Mesh::QuadIndices);

    std::cerr << std::endl;
    std::cerr << std::endl << "Message from PlyLoader::readFile(const gloost::PathType& filePath): ";
    std::cerr << std::endl << "             File \"" << filePath << "\" ";
    std::cerr << std::endl << "             successfully loaded.";
    std::cerr << std::endl << "               Number of vertices:      " << _mesh->getPositions().size();
    std::cerr << std::endl << "               Number of normals:       " << _mesh->getNormals().size();
    std::cerr << std::endl << "               Number of vertex colors: " << _mesh->getColors().size();
    std::cerr << std::endl << "               Number of texCoords:     " << _mesh->getTexCoords().size();
    std::cerr << std::endl << "               Number of triangles      " << _mesh->getTriangles().size();
    std::cerr << std::endl << "               Number of quads          " << _mesh->getQuads().size();
    std::cerr << std::endl << "               Mesh memory consumption: " << memoryConsumption/ 1024.0f /1024.0f;
    std::cerr << std::endl;
#endif


  return true;

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Reads an element line
  \remarks ...
*/

void
PlyLoader::readHeaderElement()
{
  PlyElement element;
  element.name  = _inFile.readWord();
  element.count = atoi(_inFile.readWord().c_str());

  _plyElements.push_back(element);

//  std::cerr << std::endl << "element: ";
//  std::cerr << std::endl << "        name:  " << element.name;
//  std::cerr << std::endl << "        count: " << element.count;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Reads an property and adds it to the current element
  \remarks ...
*/

void
PlyLoader::readHeaderProperty()
{
  PlyProperty property;
  std::string typeWord = _inFile.readWord();

  if (strcmp(typeWord.c_str(), "list") == 0)
  {
    property.type                = PLY_TYPE_LIST;
    property.data.list.countType = getPropertyType(_inFile.readWord().c_str());
    property.data.list.itemType  = getPropertyType(_inFile.readWord().c_str());
  }
  else
  {
    property.type = getPropertyType(typeWord.c_str());
  }

  property.name = _inFile.readWord();

  _plyElements[_plyElements.size()-1].properties.push_back(property);




//  std::cerr << std::endl << "property: ";
//  std::cerr << std::endl << "         name      : " << property.name;
//  std::cerr << std::endl << "         type      : " << property.type;
//
//  if (property.type == PLY_TYPE_LIST)
//  {
//    std::cerr << std::endl << "         countType : " << property.data.list.countType;
//    std::cerr << std::endl << "         itemType  : " << property.data.list.itemType;
//  }


}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Reads the vertices
  \remarks ...
*/

void
PlyLoader::readVertices(const PlyElement& element )
{

  for (unsigned int v=0; v!=element.count; ++v)
  {

    float x = 0;  float y = 0;  float z = 0;
    float nx = 0; float ny = 0; float nz = 0;
    float r = 0;  float g = 0;  float b = 0;  float a = 0;
    float tu = 0; float tv = 0; float tw = 0;

    bool hasNormal   = false;
    bool hasColor    = false;
    bool hasTexCoord = false;


    for (unsigned int p=0; p!=element.properties.size(); ++p)
    {
      const PlyProperty& property = element.properties[p];

      if (strcmp(property.name.c_str(), "x") == 0)
      {
        x = readPropertyValue(property.type);
      }
      else if (strcmp(property.name.c_str(), "y") == 0)
      {
        y = readPropertyValue(property.type);
      }
      else if (strcmp(property.name.c_str(), "z") == 0)
      {
        z = readPropertyValue(property.type);
      }


      else if (strcmp(property.name.c_str(), "nx") == 0)
      {
        nx = readPropertyValue(property.type);
        hasNormal = true;
      }
      else if (strcmp(property.name.c_str(), "ny") == 0)
      {
        ny = readPropertyValue(property.type);
      }
      else if (strcmp(property.name.c_str(), "nz") == 0)
      {
        nz = readPropertyValue(property.type);
      }


      else if (strcmp(property.name.c_str(), "red") == 0)
      {
        r = readPropertyValue(property.type);
        hasColor = true;

        if (property.type == PLY_TYPE_INT8)
        {
          r /= 255.0;
        }

      }
      else if (strcmp(property.name.c_str(), "green") == 0)
      {
        g = readPropertyValue(property.type);

        if (property.type == PLY_TYPE_INT8)
        {
          g /= 255.0;
        }
      }
      else if (strcmp(property.name.c_str(), "blue") == 0)
      {
        b = readPropertyValue(property.type);

        if (property.type == PLY_TYPE_INT8)
        {
          b /= 255.0;
        }
      }
      else if (strcmp(property.name.c_str(), "alpha") == 0)
      {
        a = readPropertyValue(property.type);

        if (property.type == PLY_TYPE_INT8)
        {
          a /= 255.0;
        }
      }
      else
      {
        if (strcmp(property.name.c_str(), "flags") == 0)
        {
          std::cerr << std::endl << "flags:";
        }

        readPropertyValue(property.type);
      }
    }

    _mesh->getPositions()[v] = Point3(x,y,z);

    if (hasNormal)
    {
      _mesh->getNormals().push_back(Vector3(nx, ny, nz));
    }
    if (hasColor)
    {
      _mesh->getColors().push_back(vec4(r, g, b, a));
    }
    if (hasTexCoord)
    {
      _mesh->getTexCoords().push_back(Point3(tu,tv,tw));
    }

  }

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Reads the faces
  \remarks ...
*/

void
PlyLoader::readFaces(const PlyElement& element )
{

  for (unsigned int f=0; f!=element.count; ++f)
  {

    Mesh::TriangleIndices triangleFace;
    Mesh::QuadIndices     quadFace;
    unsigned int numIndices = 0;

    for (unsigned int p=0; p!=element.properties.size(); ++p)
    {
      const PlyProperty& property = element.properties[p];

      if (strcmp(property.name.c_str(), "vertex_indices") ||
          (strcmp(property.name.c_str(), "vertex_index") && property.type == PLY_TYPE_LIST))
      {
        // number of indices per face
        numIndices = readPropertyValue(property.data.list.countType);
//        std::cerr << std::endl << "readFaces indices: " << numIndices;

        if (numIndices == 3)
        {
          for (unsigned int i=0; i!=numIndices; ++i)
          {
            triangleFace.vertexIndices[i] = readPropertyValue(property.data.list.itemType);
          }
        }
        else if (numIndices == 4)
        {
          for (unsigned int i=0; i!=numIndices; ++i)
          {
            quadFace.vertexIndices[i] = readPropertyValue(property.data.list.itemType);
          }
        }
        else
        {

          std::cerr << std::endl << "PlyLoader::readFaces(const PlyElement& element ): ";
          std::cerr << std::endl << "           skipping face with unsupported number of indices: " << numIndices;

          for (unsigned int i=0; i!=numIndices; ++i)
          {
            readPropertyValue(property.data.list.itemType);
          }
        }

      }
      else if (strcmp(property.name.c_str(), "flags"))
      {
        // flags are usupported
      }
      else
      {
        std::cerr << std::endl << "     ->>> skip Face property: " << property.name;
        std::cerr << std::endl;
        readPropertyValue(property.type);
      }
    }

    if (numIndices == 3)
    {
      _mesh->getTriangles().push_back(triangleFace);
    }
    if (numIndices == 4)
    {
      _mesh->getQuads().push_back(quadFace);
    }


  }

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Reads a numeric property value
  \remarks ...
*/

float
PlyLoader::readPropertyValue(PLY_PROPERTY_TYPE propertyType)
{
  if (propertyType == PLY_TYPE_INT8)
  {
    return (float) _inFile.readUChar8();
  }
  else if (propertyType == PLY_TYPE_INT16)
  {
//    return (float) _inFile.readInt16();
    _inFile.advanceBy(2);
    return -1;
  }
  else if (propertyType == PLY_TYPE_INT32)
  {
    return (float) _inFile.readInt32();
  }
  else if (propertyType == PLY_TYPE_FLOAT32)
  {
    return (float) _inFile.readFloat32();
  }
  else if (propertyType == PLY_TYPE_FLOAT64)
  {
    return (float) _inFile.readFloat64();
    return 0;
  }
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
  std::cerr << std::endl << "      PlyLoader::readPropertyValue(PLY_PROPERTY_TYPE propertyType)";
  std::cerr << std::endl << "      unhandled type: " << propertyType;
#endif

  return 0;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Interpretes a ply type string to internal type
  \remarks ...
*/

PlyLoader::PLY_PROPERTY_TYPE
PlyLoader::getPropertyType(const char* typeString) const
{
	if      (strcmp(typeString, "char")   == 0 ||
	         strcmp(typeString, "uchar")  == 0 ||
	         strcmp(typeString, "int8")   == 0 ||
	         strcmp(typeString, "uint8")  == 0)
	{
		return PLY_TYPE_INT8;
	}
	else if (strcmp(typeString, "uint")   == 0 ||
	         strcmp(typeString, "int16")  == 0 ||
	         strcmp(typeString, "uint16") == 0 ||
	         strcmp(typeString, "short")  == 0 ||
	         strcmp(typeString, "ushort") == 0)
	{
		return PLY_TYPE_INT16;
	}
	else if (strcmp(typeString, "int")    == 0 ||
	         strcmp(typeString, "long")   == 0 ||
	         strcmp(typeString, "ulong")  == 0 ||
	         strcmp(typeString, "int32")  == 0 ||
	         strcmp(typeString, "uint32") == 0)
	{
		return PLY_TYPE_INT32;
	}
	else if (strcmp(typeString, "float")   == 0 ||
	         strcmp(typeString, "float32") == 0)
	{
		return PLY_TYPE_FLOAT32;
	}
	else if (strcmp(typeString, "float64") == 0 ||
	         strcmp(typeString, "double")  == 0)
	{
		return PLY_TYPE_FLOAT64;
	}
	else if ( strcmp(typeString, "list") == 0 )
	{
		return PLY_TYPE_LIST;
	}
	else
	{
		return PLY_TYPE_UNKNOWN;
	}
}


///////////////////////////////////////////////////////////////////////////////






} // namespace gloost



