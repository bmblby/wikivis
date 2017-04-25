
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
#include <gloost/gloostConfig.h>
#include <gloost/gl/ShaderProgram.h>
#include <gloost/BinaryFile.h>

// cpp includes
#include <string>
#include <iostream>

namespace gloost
{
namespace gl
{

/**
  \class   ShaderProgram

  \brief   GLSL shader supporting openGl >= 4.0 features

  \author  Felix Weiszig, Henning Gruendl
  \date    November 2011
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   class factory
  \remarks ...
*/

/*static*/
ShaderProgram::shared_ptr
ShaderProgram::create()
{
  auto instance = shared_ptr(new ShaderProgram());
  return instance;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

ShaderProgram::ShaderProgram():
  MultiGlContextEntity(1),
  _components(),
  _componentCompileError(GL_FALSE)
{
  // insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

ShaderProgram::~ShaderProgram()
{
  for (unsigned i=0; i!=GLOOST_SYSTEM_NUM_RENDER_CONTEXTS; ++i)
  {
    removeFromContext(i);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compatibility call
*/

/*virtual*/
bool
ShaderProgram::attachShaderFile(unsigned shaderType, const std::string& fileName)
{
  return attachShaderFile(shaderType, gloost::PathType(fileName));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   attaches a specific shader type to be bound
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
ShaderProgram::attachShaderFile(unsigned shaderType, const gloost::PathType& fileName)
{
  _components[shaderType] = ShaderComponent(fileName, shaderType, "");
  return readSourceFile(_components[shaderType]);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   attaches a specific shader type to be bound
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
ShaderProgram::attachShaderSource(unsigned shaderType, const std::string& source)
{
  _components[shaderType] = ShaderComponent(gloost::PathType(""), shaderType, source);
  return readSourceFile(_components[shaderType]);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   reloads all shader source files and updates the ShaderProgram on the GPU
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ShaderProgram::reloadShaders()
{
  std::map<unsigned, ShaderComponent>::iterator componentIt    = _components.begin();
  std::map<unsigned, ShaderComponent>::iterator componentEndIt = _components.end();

  for (; componentIt!=componentEndIt; ++componentIt)
  {
    readSourceFile((*componentIt).second);
  }

  for (unsigned i=0; i!=GLOOST_SYSTEM_NUM_RENDER_CONTEXTS; ++i)
  {
    removeFromContext(i);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the ShaderProgram
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
ShaderProgram::initInContext(unsigned contextId)
{

  if (handleInContextExists(contextId))
  {
    removeFromContext(contextId);
  }


  std::map<unsigned, ShaderComponent>::iterator componentIt    = _components.begin();
  std::map<unsigned, ShaderComponent>::iterator componentEndIt = _components.end();

  for (; componentIt!=componentEndIt; ++componentIt)
  {
    compileShader((*componentIt).second);
  }

  unsigned shaderHandle = glCreateProgram();


  /// make a association between the context and the GL handle of this shader
  setResourceHandleForContext(shaderHandle, contextId);


  // attach components to shader program
  componentIt = _components.begin();

  for (; componentIt!=componentEndIt; ++componentIt)
  {
    /// attache programs to shaderh handle
    glAttachShader( shaderHandle, (*componentIt).second._handle );
  }


  /// link components to a shader program
  glLinkProgram(shaderHandle);

//  int status;
//  glGetProgramiv(shaderHandle, GL_LINK_STATUS, &status);
//  if (!status)
//  {
//    validateShaderProgram(contextId);
//  }


#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  if (!_componentCompileError)
  {
    std::cout << std::endl;
    std::cout << std::endl << "Message from Shader::compileToProgram() on SharedResource " << getSharedResourceId() << ":";
    std::cout << std::endl << "             Successfully compiled and linked.";

    componentIt    = _components.begin();
    for (; componentIt!=componentEndIt; ++componentIt)
    {
      /// attache programs to shaderh handle
      std::cerr << std::endl << "             " << (*componentIt).second._fileName;
    }

    std::cout << std::endl;
    return true;
  }
#endif

  return false;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief Removes a ShaderProgram from a context
  \param contextId id of the context, the shader will be removed from. Use 0 or no
         argument if you work with only one context.
  \remarks If you call set() on this ShaderProgram, it will be automaticly initialized
           for the context again
*/

/*virtual*/
void
ShaderProgram::removeFromContext(unsigned contextId)
{
  if (handleInContextExists(contextId))
  {
    glDeleteObjectARB(getResourceHandleForContext(contextId));
    setResourceHandleForContext(0, contextId);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   loads the source file for a shader
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
ShaderProgram::readSourceFile(ShaderComponent& component)
{
  if (component._fileName.empty()) return true;

  unsigned char* buffer       = 0;
  size_t         bufferSizeInByte = 0;

  if (!gloost::BinaryFile::read(component._fileName, buffer, bufferSizeInByte))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cerr << std::endl;
    std::cerr << std::endl << "ERROR from ShaderProgram::readSourceFile() on SharedResource " << getSharedResourceId() << ":";
    std::cerr << std::endl << "           Could NOT load source file \"" << component._fileName << "\"";
    std::cerr << std::endl;
#endif
    delete buffer;
    return false;
  }

  char* shaderSource = new char[bufferSizeInByte+1];
  memcpy(shaderSource, buffer, bufferSizeInByte);
  shaderSource[bufferSizeInByte] = '\0';

  component._source = std::string(shaderSource);

  delete buffer;
  delete [] shaderSource;
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief compiles a shader and throws error if any
  \remarks ...
*/

void
ShaderProgram::compileShader(ShaderComponent& component)
{
  component._handle = glCreateShader(component._type);

  const char* source = component._source.c_str();
  glShaderSource(component._handle, 1, &source, NULL);

  glCompileShader(component._handle);
  glGetShaderiv(component._handle, GL_COMPILE_STATUS, &component._compileStatus);

  if (component._compileStatus == GL_FALSE)
  {
    _componentCompileError = true;
  }

  // validate the vertex shader and print compiler log
  validateShaderComponent(component);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief Validates a shader by using glGetShaderInfoLog
  \remarks On ATI cards there will be always a message that the shader was build correctly
*/
void
ShaderProgram::validateShaderComponent(ShaderComponent& component)
{
  const unsigned messageSize = 2048;
  char           messageBuffer[messageSize];
  memset(messageBuffer, 0, messageSize);
  GLsizei length = 0;

  glGetShaderInfoLog(component._handle, messageSize, &length, messageBuffer);

  component._lastCompileMessage = messageBuffer;

  if (length > 0)
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in ShaderProgram::validateShaderComponent:";
    std::cout << std::endl << "          Compile error(s)/warning(s) in file \"" << component._fileName << "\" !";
    std::cout << std::endl << messageBuffer;
    std::cout << std::endl;
    std::cout << std::endl;
#endif
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Validates a shader program by using glGetProgramInfoLog
  \remarks On ATI cards there will be always a message that the shader was build correctly
*/
void
ShaderProgram::validateShaderProgram(unsigned contextId)
{
  const unsigned messageSize = 2048;
  char           messageBuffer[messageSize];
  memset(messageBuffer, 0, messageSize);
  GLsizei length = 0;

  gloostId handle = getResourceHandleForContext(contextId);
  glGetProgramInfoLog(handle, messageSize, &length, messageBuffer);

//  component._lastCompileMessage = messageBuffer;

  if (length > 0)
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in ShaderProgram::validateShaderProgram:";
    std::cout << std::endl << "          Following ERRORS accured:";
    std::cout << std::endl << messageBuffer;
    std::cout << std::endl;
    std::cout << std::endl;
#endif
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the OpenGL shader handle for a specified context

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

unsigned
ShaderProgram::getHandle(unsigned contextId)
{
  if (!handleInContextExists(contextId))
  {
    initInContext(contextId);
  }
  return getResourceHandleForContext(contextId);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the uniform location for an uniforms name

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

int
ShaderProgram::getUniformLocation(const std::string& name, unsigned contextId)
{
  return glGetUniformLocation(getHandle(contextId), name.c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns true if a uniform name is used withing the ShaderProgram

  \param name name of the Uniform variable to check for
         contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

bool
ShaderProgram::isUniformDefined(const std::string& name, unsigned contextId)
{
  return (-1 != glGetUniformLocation(getHandle(contextId), name.c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, float val, unsigned contextId)
{
  glUniform1f(getUniformLocation(name, contextId), val);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, int val, unsigned contextId)
{
  glUniform1i(getUniformLocation(name, contextId), val);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the uniform location for an uniforms name

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, bool val, unsigned contextId)
{
  glUniform1i(getUniformLocation(name, contextId), val);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, float x, float y, float z, unsigned contextId)
{
  glUniform3f(getUniformLocation(name, contextId), x, y, z);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const vec3& v, unsigned contextId)
{
  glUniform3f(getUniformLocation(name, contextId), v.x, v.y, v.z);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const vec4& v, unsigned contextId)
{
  glUniform4f(getUniformLocation(name, contextId), v.r ,v.g, v.b, v.a);
//  glUniform4fv(getUniformLocation(name, contextId), 4, (const GLfloat*) &v.r);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const vec2& v, unsigned contextId)
{
  glUniform2f(getUniformLocation(name, contextId), v.u, v.v);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const mat3& m, unsigned contextId)
{
  glUniformMatrix3fv(getUniformLocation(name, contextId), 1, GL_FALSE, m.data);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const mat4& m, unsigned contextId)
{
  glUniformMatrix4fv(getUniformLocation(name, contextId), 1, GL_FALSE, m.data);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const Vector2& v, unsigned contextId)
{
  glUniform2f(getUniformLocation(name, contextId), v[0], v[1]);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const Vector3& v, unsigned contextId)
{
  glUniform3f(getUniformLocation(name, contextId), v[0], v[1], v[2]);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const Point3& p, unsigned contextId)
{
  glUniform3f(getUniformLocation(name, contextId), p[0], p[1], p[2]);
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief sets a uniform variable

  \param contextId The context id of the context you want the handle from. Use 0 or no
         argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/

void
ShaderProgram::setUniform(const std::string& name, const Matrix& m, unsigned contextId)
{
#ifdef GLOOST_SYSTEM_MATH_WITH_FLOAT
  glUniformMatrix4fv(getUniformLocation(name, contextId), 1, 0, (GLfloat*)(m.data()));
#else
  mat4 matrix(m);
  glUniformMatrix4fv(getUniformLocation(name, contextId), 1, 0, (GLfloat*)(matrix.data));
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Binds/sets the shader into the specified GL state

  \param   contextId The context id of the context you want the shader set to. Use 0 or no
           argument if you work with only one context.

  \remarks The shader will be initialized for the context if not allready done
*/
void
ShaderProgram::use(unsigned contextId)
{
  if (!handleInContextExists(contextId))
  {
    initInContext(contextId);
  }
  // SetShader
  glUseProgram(getResourceHandleForContext(contextId));
}

///////////////////////////////////////////////////////////////////////////////////////////////

/**
  \brief Disable all GLSL shader rendering and fall back to the standard pipeline

  \param contextId The context id of the context you want the shader set to. Use 0 or no
         argument if you work with only one context.

  \remarks all texture units will be cleared and all texture matrices will be set to identity.
*/

/*static*/
void
ShaderProgram::disable()
{
  glUseProgram(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace gl
} // namespace gloost

