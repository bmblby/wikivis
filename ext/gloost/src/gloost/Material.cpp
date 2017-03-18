
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
#include <gloost/Material.h>
#include <gloost/TextureManager.h>



// cpp includes




namespace gloost
{



///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Material::Material():
   _map_float(),
   _map_vec2(),
   _map_vec3(),
   _map_vec4(),
   _map_int(),
   _map_ivec2(),
   _map_ivec3(),
   _map_ivec4(),
   _map_bool(),
   _map_bvec2(),
   _map_bvec3(),
   _map_bvec4(),
   _map_mat2(),
   _map_mat3(),
   _map_mat4(),
   _map_sampler(),
   _map_string(),
   _map_customResources()

{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


 /// class destructor

Material::~Material()
{
  // TextureManager
  TextureManager* texManager = TextureManager::getInstance();

  // drop all texture references
  std::map<std::string, gloostId>::iterator iter = _map_sampler.begin();

  for ( ; iter != _map_sampler.end(); ++iter)
  {
    texManager->dropReference((*iter).second);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a float value to the shader

void
Material::set_float(const std::string& uniformName, float value)
{
  _map_float[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

float
Material::get_float(const std::string& uniformName)
{
  std::map<std::string, float>::iterator pos = _map_float.find(uniformName);

  if (pos == _map_float.end())
  {
    return 0.0;
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a vec2 value to the shader

void
Material::set_vec2(const std::string& uniformName, vec2 value)
{
  _map_vec2[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

vec2
Material::get_vec2(const std::string& uniformName)
{
  std::map<std::string, vec2>::iterator pos = _map_vec2.find(uniformName);

  if (pos == _map_vec2.end())
  {
    return vec2();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a vec3 value to the shader

void
Material::set_vec3(const std::string& uniformName, vec3 value)
{
  _map_vec3[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

vec3
Material::get_vec3(const std::string& uniformName)
{
  std::map<std::string, vec3>::iterator pos = _map_vec3.find(uniformName);

  if (pos == _map_vec3.end())
  {
    return vec3();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a vec4 value to the shader

void
Material::set_vec4(const std::string& uniformName, vec4 value)
{
  _map_vec4[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

vec4
Material::get_vec4(const std::string& uniformName)
{
  std::map<std::string, vec4>::iterator pos = _map_vec4.find(uniformName);

  if (pos == _map_vec4.end())
  {
    return vec4();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a int value to the shader

void
Material::set_int(const std::string& uniformName, int value)
{
  _map_int[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

int
Material::get_int(const std::string& uniformName)
{
  std::map<std::string, int>::iterator pos = _map_int.find(uniformName);

  if (pos == _map_int.end())
  {
    return 0;
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a ivec2 value to the shader

void
Material::set_ivec2(const std::string& uniformName, ivec2 value)
{
  _map_ivec2[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

ivec2
Material::get_ivec2(const std::string& uniformName)
{
  std::map<std::string, ivec2>::iterator pos = _map_ivec2.find(uniformName);

  if (pos == _map_ivec2.end())
  {
    return ivec2();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a ivec3 value to the shader

void
Material::set_ivec3(const std::string& uniformName, ivec3 value)
{
  _map_ivec3[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

ivec3
Material::get_ivec3(const std::string& uniformName)
{
  std::map<std::string, ivec3>::iterator pos = _map_ivec3.find(uniformName);

  if (pos == _map_ivec3.end())
  {
    return ivec3();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a ivec4 value to the shader

void
Material::set_ivec4(const std::string& uniformName, ivec4 value)
{
  _map_ivec4[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

ivec4
Material::get_ivec4(const std::string& uniformName)
{
  std::map<std::string, ivec4>::iterator pos = _map_ivec4.find(uniformName);

  if (pos == _map_ivec4.end())
  {
    return ivec4();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a bool value to the shader

void
Material::set_bool(const std::string& uniformName, bool value)
{
  _map_bool[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

bool
Material::get_bool(const std::string& uniformName)
{
  std::map<std::string, bool>::iterator pos = _map_bool.find(uniformName);

  if (pos == _map_bool.end())
  {
    return 0;
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a bvec2 value to the shader

void
Material::set_bvec2(const std::string& uniformName, bvec2 value)
{
  _map_bvec2[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

bvec2
Material::get_bvec2(const std::string& uniformName)
{
  std::map<std::string, bvec2>::iterator pos = _map_bvec2.find(uniformName);

  if (pos == _map_bvec2.end())
  {
    return bvec2();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a bvec3 value to the shader

void
Material::set_bvec3(const std::string& uniformName, bvec3 value)
{
  _map_bvec3[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

bvec3
Material::get_bvec3(const std::string& uniformName)
{
  std::map<std::string, bvec3>::iterator pos = _map_bvec3.find(uniformName);

  if (pos == _map_bvec3.end())
  {
    return bvec3();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a bvec4 value to the shader

void
Material::set_bvec4(const std::string& uniformName, bvec4 value)
{
  _map_bvec4[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

bvec4
Material::get_bvec4(const std::string& uniformName)
{
  std::map<std::string, bvec4>::iterator pos = _map_bvec4.find(uniformName);

  if (pos == _map_bvec4.end())
  {
    return bvec4();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a mat2 value to the shader

void
Material::set_mat2(const std::string& uniformName, mat2 value)
{
  _map_mat2[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

mat2
Material::get_mat2(const std::string& uniformName)
{
  std::map<std::string, mat2>::iterator pos = _map_mat2.find(uniformName);

  if (pos == _map_mat2.end())
  {
    return mat2();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a mat3 value to the shader

void
Material::set_mat3(const std::string& uniformName, mat3 value)
{
  _map_mat3[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

mat3
Material::get_mat3(const std::string& uniformName)
{
  std::map<std::string, mat3>::iterator pos = _map_mat3.find(uniformName);

  if (pos == _map_mat3.end())
  {
    return mat3();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Add a mat4 value to the shader

void
Material::set_mat4(const std::string& uniformName, mat4 value)
{
  _map_mat4[uniformName] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

mat4
Material::get_mat4(const std::string& uniformName)
{
  std::map<std::string, mat4>::iterator pos = _map_mat4.find(uniformName);

  if (pos == _map_mat4.end())
  {
    return mat4();
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Adds a texture id from the gloost::TextureManager to be used as texture in the shader
  \remarks The UniformSet will take a reference of the Texture
*/

void
Material::set_sampler(const std::string& uniformName, gloostId textureGid)
{
  std::map<std::string, gloostId>::iterator pos = _map_sampler.find(uniformName);

  if (pos != _map_sampler.end())
  {
    // this is the same texture -> do nothing and return
    if (pos->second == textureGid)
    {
      return;
    }
    // drop the reference of the old texture, assign the new one and return
    else
    {
      gloost::TextureManager::get()->dropReference(pos->second);
      pos->second = textureGid;
      gloost::TextureManager::get()->getTextureReference(textureGid);
      return;
    }
  }

  gloost::TextureManager::get()->getTextureReference(textureGid);
  _map_sampler[uniformName] = textureGid;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

gloostId
Material::get_sampler(const std::string& uniformName)
{
  std::map<std::string, gloostId>::iterator pos = _map_sampler.find(uniformName);

  if (pos == _map_sampler.end())
  {
    return 0;
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// add a string

void
Material::set_string(const std::string& name, const std::string& value)
{
  _map_string[name] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

std::string
Material::get_string(const std::string& name)
{
  std::map<std::string, std::string>::iterator pos = _map_string.find(name);

  if (pos == _map_string.end())
  {
    return std::string("null");
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// add a custom attribute

void
Material::set_customResource(const std::string& name, const Material::CustomResource& value)
{
  _map_customResources[name] = value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// add a custom attribute

void
Material::set_customResource(const std::string& name, const std::string& path, unsigned type)
{
  _map_customResources[name] = CustomResource(path, type);
}


///////////////////////////////////////////////////////////////////////////////////////////////


 ///

Material::CustomResource
Material::get_customResource(const std::string& name)
{
  std::map<std::string, CustomResource>::iterator pos = _map_customResources.find(name);

  if (pos == _map_customResources.end())
  {
    return CustomResource("null", 0);
  }
  else
  {
    return (*pos).second;
  }
}


///////////////////////////////////////////////////////////////////////////////




} // namespace gloost

