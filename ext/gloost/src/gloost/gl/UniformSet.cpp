
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
#include <gloost/gl/UniformSet.h>
#include <gloost/TextureManager.h>
#include <gloost/gl/ShaderProgram.h>

#include <GL/glew.h>

namespace gloost
{

namespace gl
{

/* static */
unsigned UniformSet::_current_texture_offset = 0u;

///////////////////////////////////////////////////////////////////////////////

 /// class constructor

UniformSet::UniformSet():
  Material()
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

 /// class destructor

UniformSet::~UniformSet()
{

}

///////////////////////////////////////////////////////////////////////////////

 /// set uniforms to shader

void
UniformSet::applyToShader(ShaderProgram* shader, unsigned contextId)
{
  _current_texture_offset = 0u;
  addToShader(shader->getHandle(contextId), contextId);
}

///////////////////////////////////////////////////////////////////////////////

 /// set uniforms to shader

void
UniformSet::applyToShader(const std::shared_ptr<ShaderProgram>& shader, unsigned contextId)
{
  _current_texture_offset = 0u;
  addToShader(shader->getHandle(contextId), contextId);
}

///////////////////////////////////////////////////////////////////////////////

 /// set uniforms to shader

void
UniformSet::applyToShader(const unsigned shaderHandle, unsigned contextId)
{
  _current_texture_offset = 0u;
  addToShader(shaderHandle, contextId);
}

///////////////////////////////////////////////////////////////////////////////

 /// set uniforms to shader

void
UniformSet::addToShader(ShaderProgram* shader, unsigned contextId)
{
  addToShader(shader->getHandle(contextId), contextId);
}

///////////////////////////////////////////////////////////////////////////////

 /// set uniforms to shader

void
UniformSet::addToShader(const unsigned shaderHandle, unsigned contextId)
{
  // TextureManager
  TextureManager* texManager = TextureManager::getInstance();

  // Uniform handle
  unsigned uniformPos = 0u;

  // Bind floats
  if(_map_float.size())
  {
    std::map<std::string, float>::const_iterator index = _map_float.begin();
    std::map<std::string, float>::const_iterator end   = _map_float.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform1f(uniformPos, (*index).second);
    }
  }

  // Bind vec2
  if(_map_vec2.size())
  {
    std::map<std::string, vec2>::const_iterator index = _map_vec2.begin();
    std::map<std::string, vec2>::const_iterator end   = _map_vec2.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform2f(uniformPos, (*index).second.u, (*index).second.v);
    }
  }

  // Bind vec3
  if(_map_vec3.size())
  {
    std::map<std::string, vec3>::const_iterator index = _map_vec3.begin();
    std::map<std::string, vec3>::const_iterator end   = _map_vec3.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform3f(uniformPos, (*index).second.x, (*index).second.y, (*index).second.z);
    }
  }

  // Bind vec4
  if(_map_vec4.size())
  {
    std::map<std::string, vec4>::const_iterator index = _map_vec4.begin();
    std::map<std::string, vec4>::const_iterator end   = _map_vec4.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform4f(uniformPos, (*index).second.r, (*index).second.g,
                              (*index).second.b, (*index).second.a);
    }
  }

  //////////////////////////////////////////////////////////////////////

  // Bind int
  if(_map_int.size())
  {
    std::map<std::string, int>::const_iterator index = _map_int.begin();
    std::map<std::string, int>::const_iterator end   = _map_int.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform1i(uniformPos, (*index).second);
    }
  }

  // Bind ivec2
  if(_map_ivec2.size())
  {
    std::map<std::string, ivec2>::const_iterator index = _map_ivec2.begin();
    std::map<std::string, ivec2>::const_iterator end   = _map_ivec2.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform2i(uniformPos, (*index).second.u, (*index).second.v);
    }
  }

  // Bind ivec3
  if(_map_ivec3.size())
  {
    std::map<std::string, ivec3>::const_iterator index = _map_ivec3.begin();
    std::map<std::string, ivec3>::const_iterator end   = _map_ivec3.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform3i(uniformPos, (*index).second.x, (*index).second.y, (*index).second.z);
    }
  }

  // Bind ivec4
  if(_map_ivec4.size())
  {
    std::map<std::string, ivec4>::const_iterator index = _map_ivec4.begin();
    std::map<std::string, ivec4>::const_iterator end   = _map_ivec4.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform4i(uniformPos, (*index).second.r, (*index).second.g,
                              (*index).second.b, (*index).second.a);
    }
  }

  //////////////////////////////////////////////////////////////////////

  // Bind bool
  if(_map_bool.size())
  {
    std::map<std::string, bool>::const_iterator index = _map_bool.begin();
    std::map<std::string, bool>::const_iterator end   = _map_bool.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform1i(uniformPos, (*index).second);
    }
  }

  // Bind bvec2
  if(_map_bvec2.size())
  {
    std::map<std::string, bvec2>::const_iterator index = _map_bvec2.begin();
    std::map<std::string, bvec2>::const_iterator end   = _map_bvec2.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform2i(uniformPos, (*index).second.u, (*index).second.v);
    }
  }

  // Bind bvec3
  if(_map_bvec3.size())
  {
    std::map<std::string, bvec3>::const_iterator index = _map_bvec3.begin();
    std::map<std::string, bvec3>::const_iterator end   = _map_bvec3.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform3i(uniformPos, (*index).second.x, (*index).second.y, (*index).second.z);
    }
  }

  // Bind bvec4
  if(_map_bvec4.size())
  {
    std::map<std::string, bvec4>::const_iterator index = _map_bvec4.begin();
    std::map<std::string, bvec4>::const_iterator end   = _map_bvec4.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniform4i(uniformPos, (*index).second.r, (*index).second.g,
                              (*index).second.b, (*index).second.a);
    }
  }

  //////////////////////////////////////////////////////////////////////

  // Bind mat2
  if(_map_mat2.size())
  {
    std::map<std::string, mat2>::const_iterator index = _map_mat2.begin();
    std::map<std::string, mat2>::const_iterator end   = _map_mat2.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniformMatrix2fv(uniformPos, 4, 0, (GLfloat*)&((*index).second));
    }
  }

  // Bind mat3
  if(_map_mat3.size())
  {
    std::map<std::string, mat3>::const_iterator index = _map_mat3.begin();
    std::map<std::string, mat3>::const_iterator end   = _map_mat3.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniformMatrix3fv(uniformPos, 1, false, (GLfloat*)&(*index).second  );
    }
  }

  // Bind mat4
  if(_map_mat4.size())
  {
    std::map<std::string, mat4>::const_iterator index = _map_mat4.begin();
    std::map<std::string, mat4>::const_iterator end   = _map_mat4.end();

    for (; index != end; ++index)
    {
      uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
      glUniformMatrix4fv(uniformPos, 1, 0, (GLfloat*)&((*index).second));
    }
  }

  //////////////////////////////////////////////////////////////////////

  // Bind sampler
  if(_map_sampler.size())
  {
    std::map<std::string, gloostId>::const_iterator index = _map_sampler.begin();
    std::map<std::string, gloostId>::const_iterator ende  = _map_sampler.end();

    for (; index != ende; index++ )
    {
      Texture* texture = texManager->getTextureWithoutRefcount((*index).second);

      if (texture)
      {
        texture->bind(GL_TEXTURE0+_current_texture_offset, contextId);
        uniformPos = glGetUniformLocation (shaderHandle, (*index).first.c_str());
        glUniform1i(uniformPos, _current_texture_offset);
        ++_current_texture_offset;
      }

    }
  }
}

///////////////////////////////////////////////////////////////////////////////

 ///

} // namespace gl
} // namespace gloost
