
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



/// gloost system includes
#include <gloost/ObjMatFile.h>
#include <gloost/TextureManager.h>


/// cpp includes
#include <string>
#include <iostream>


namespace gloost
{

////////////////////////////////////////////////////////////////////////////////


  /// class constructor

ObjMatFile::ObjMatFile():
    SharedResource(),
    _filePath("0"),
    _materialMap()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


  /// class destructor

ObjMatFile::~ObjMatFile()
{

}


////////////////////////////////////////////////////////////////////////////////


  /// ...

bool
ObjMatFile::load(const std::string& filePath)
{
  _filePath = filePath;

  std::string baseFilePath = gloost::helper::pathToBasePath(filePath);

  std::ifstream infile;
  infile.open(_filePath.c_str());

  if (!infile)
  {

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl;
    std::cerr << std::endl << "Warning from ObjMatFile::load:";
    std::cerr << std::endl << "             Cant open \"" << _filePath << "\"!";
    std::cerr << std::endl;
#endif

    return 0;
  }


#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cerr << std::endl;
  std::cerr << std::endl << "Message from ObjMatFile::load:";
  std::cerr << std::endl << "             Loading \"" << _filePath << "\".";
  std::cerr << std::endl << "baseFilePath: " << baseFilePath;
  std::cerr << std::endl;
#endif

  std::string token;
  std::string currentMaterialName;

  while(infile >> token)
  {

    if(token == "newmtl")
    {
      infile >> currentMaterialName;
      _materialMap[currentMaterialName] = gl::UniformSet();

    }

    else if(token == "Ka")
    {
      Point3 component;
      infile >> component[0];
      infile >> component[1];
      infile >> component[2];
      _materialMap[currentMaterialName].set_vec4("v4_ambientColor", vec4(component[0],component[1],component[2], 1.0));
    }
    else if(token == "Kd")
    {
      Point3 component;
      infile >> component[0];
      infile >> component[1];
      infile >> component[2];
      _materialMap[currentMaterialName].set_vec4("v4_diffuseColor", vec4(component[0],component[1],component[2], 1.0));
    }
    else if(token == "Ks")
    {
      Point3 component;
      infile >> component[0];
      infile >> component[1];
      infile >> component[2];
      _materialMap[currentMaterialName].set_vec4("v4_specularColor", vec4(component[0],component[1],component[2], 1.0));
    }
    else if(token == "illum")
    {
      float component = 0.0;
      infile >> component;
      _materialMap[currentMaterialName].set_float("f_illum", component);
    }
    else if(token == "Ns")
    {
      float component = 0.0;
      infile >> component;
      _materialMap[currentMaterialName].set_float("v4_specularCoeff", component);
    }
    else if(token == "map_Ka")
    {
      std::string texturePath;
      infile >> texturePath;

      unsigned texId = gloost::TextureManager::get()->createTexture(baseFilePath+texturePath);
      _materialMap[currentMaterialName].set_sampler("map_ambient", texId);
    }
    else if(token == "map_Kd")
    {
      std::string texturePath;
      infile >> texturePath;

      unsigned texId = gloost::TextureManager::get()->createTexture(baseFilePath+texturePath);
      _materialMap[currentMaterialName].set_sampler("map_diffuse", texId);
    }
    else if(token == "map_Ks")
    {
      std::string texturePath;
      infile >> texturePath;

      unsigned texId = gloost::TextureManager::get()->createTexture(baseFilePath+texturePath);
      _materialMap[currentMaterialName].set_sampler("map_specular", texId);
    }
    else if(token == "map_d")
    {
      std::string texturePath;
      infile >> texturePath;

      unsigned texId = gloost::TextureManager::get()->createTexture(baseFilePath+texturePath);
      _materialMap[currentMaterialName].set_sampler("map_alpha", texId);
    }
    else if(token == "bump" || token == "map_bump")
    {
      std::string texturePath;
      infile >> texturePath;

      unsigned texId = gloost::TextureManager::get()->createTexture(baseFilePath+texturePath);
      _materialMap[currentMaterialName].set_sampler("map_bump", texId);
    }
    else if(token == "normal" || token == "map_normal")
    {
      std::string texturePath;
      infile >> texturePath;

      unsigned texId = gloost::TextureManager::get()->createTexture(baseFilePath+texturePath);
      _materialMap[currentMaterialName].set_sampler("map_normal", texId);
    }
  }

  infile.close();

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout << std::endl;
  std::cout << std::endl << "Message from ObjMatFile::load:";
  std::cout << std::endl << "             \"" << _filePath << "\" succesfully loaded";
  std::cout << std::endl << "              " << _materialMap.size() << " materials:";

  std::map<std::string, gl::UniformSet>::iterator matIt    = _materialMap.begin();
  std::map<std::string, gl::UniformSet>::iterator matItEnd = _materialMap.end();

  for (; matIt!=matItEnd; ++matIt)
  {
    std::cout << std::endl << "                - \"" << (*matIt).first << "\"";
  }

  std::cout << std::endl;
  std::flush(std::cout);
#endif

  return true;
}


////////////////////////////////////////////////////////////////////////////////


  /// returns a gloost::Material Pointer for a name or 0 if the material was not found

gl::UniformSet&
ObjMatFile::getMaterial(const std::string& materialName)
{
  std::map<std::string, gl::UniformSet>::iterator pos = _materialMap.find(materialName);

  if (pos != _materialMap.end())
  {
    return (*pos).second;
  }

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cout << std::endl;
    std::cout << std::endl << "WARNING from ObjMatFile::getMaterial:";
    std::cout << std::endl << "             Material with name \"" << materialName << "\" was not";
    std::cout << std::endl << "             found in this ObjMatFile instance.";

    if (_filePath != std::string("0"))
    {
      std::cout << std::endl << "             loaded from defined in  \"" << _filePath << "\".";
    }

    std::cout << std::endl;
    std::flush(std::cout);
#endif

  if (_materialMap.find("default") == _materialMap.end())
  {
    _materialMap["default"] = gl::UniformSet();
  }
  return _materialMap["default"];
}


////////////////////////////////////////////////////////////////////////////////


  /// returns a map with the material names as key and a pointer to a Material as value

std::map<std::string, gl::UniformSet>&
ObjMatFile::getMaterials()
{
  return _materialMap;
}


////////////////////////////////////////////////////////////////////////////////


  /// returns a map with the material names as key and a pointer to a Material as value

const std::map<std::string, gl::UniformSet>&
ObjMatFile::getMaterials() const
{
  return _materialMap;
}


////////////////////////////////////////////////////////////////////////////////


  /// add a Material to this mat file

void
ObjMatFile::addMaterial(const std::string& name, const gl::UniformSet& material)
{
  _materialMap[name] = material;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost

