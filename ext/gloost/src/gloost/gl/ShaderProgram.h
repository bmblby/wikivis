
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



#ifndef H_GLOOST_SHADERPROGRAM
#define H_GLOOST_SHADERPROGRAM



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/gl/MultiGlContextEntity.h>


// cpp includes
#include <string>
#include <map>
#include <memory>
#include <GL/glew.h>



#define GLOOST_SHADERPROGRAM_UNDEFINED_SHADER       0
#define GLOOST_SHADERPROGRAM_VERTEX_SHADER          GL_VERTEX_SHADER
#define GLOOST_SHADERPROGRAM_GEOMETRY_SHADER        GL_GEOMETRY_SHADER
#define GLOOST_SHADERPROGRAM_FRAGMENT_SHADER        GL_FRAGMENT_SHADER
#define GLOOST_SHADERPROGRAM_TESS_CONTROL_SHADER    GL_TESS_CONTROL_SHADER
#define GLOOST_SHADERPROGRAM_TESS_EVALUATION_SHADER GL_TESS_EVALUATION_SHADER
#define GLOOST_SHADERPROGRAM_COMPUTE_SHADER         GL_COMPUTE_SHADER



namespace gloost
{

namespace gl
{


  //  GLSL shader supporting openGl 4.0 features

class ShaderProgram: public MultiGlContextEntity
{
	public:

    /// a std::shared_ptr of an ShaderProgram instance
    typedef std::shared_ptr<ShaderProgram>       shared_ptr;
    typedef std::shared_ptr<const ShaderProgram> const_shared_ptr;

	  struct ShaderComponent
	  {
	    ShaderComponent()
	    {
        _fileName           = "no name";
        _type               = GLOOST_SHADERPROGRAM_UNDEFINED_SHADER;
        _source             = "";
        _handle             = 0;
        _compileStatus      = false;
        _lastCompileMessage = "";
	    }

	    ShaderComponent(const gloost::PathType& fileName, unsigned type, const std::string& source)
	    {
        _fileName           = fileName;
        _type               = type;
        _source             = source;
        _handle             = 0;
        _compileStatus      = false;
        _lastCompileMessage = "";
	    }

	    ShaderComponent(const std::string& fileName, unsigned type, const std::string& source):
	      ShaderComponent(gloost::PathType(fileName), type, source)
	    {
	    }

	    ShaderComponent(const ShaderComponent& component)
	    {
        _fileName           = component._fileName;
        _type               = component._type;
        _source             = component._source;
        _handle             = component._handle;
        _compileStatus      = component._compileStatus;
        _lastCompileMessage = component._lastCompileMessage;
	    }

	    gloost::PathType  _fileName;
	    unsigned          _type;
	    std::string       _source;
	    unsigned          _handle;
	    int               _compileStatus;
	    std::string       _lastCompileMessage;
	  };

    // class factory
    static shared_ptr create ();

    // class constructor
    ShaderProgram();

    // class destructor
	  virtual ~ShaderProgram();


	  // attaches a specific shader type to be bound
    bool attachShaderFile (unsigned shaderType, const std::string& fileName);

	  // attaches a specific shader type to be bound
    bool attachShaderFile (unsigned shaderType, const gloost::PathType& fileName);

	  // attaches a specific shader type to be bound
    bool attachShaderSource (unsigned shaderType, const std::string& fileName);


    // reloads all shader source files and updates the ShaderProgram on the GPU
    void reloadShaders();


    // inits the ShaderProgram for a specific context
	  virtual bool initInContext(unsigned contextId = 0);

    // remove a resource from a context
	  virtual void removeFromContext(unsigned contextId = 0);


    // Return the shader handle
    unsigned getHandle(unsigned contextId = 0);

    // Set/unset the Shader for following geometry
    void use(unsigned contextId = 0);

    // disable all GLSL shader rendering
    static void disable();


    // loads the source file for a shader
    bool readSourceFile(ShaderComponent& component);

    // compiles a shader and throws error if any
	  void compileShader(ShaderComponent& component);

    // validates a shader component by using glGetShaderInfoLog
    void validateShaderComponent(ShaderComponent& component);

    // validates a shader program by using glGetShaderInfoLog
    void validateShaderProgram(unsigned contextId = 0);


    // returns a uniform location for uniforms name or -1 if name names no uniform variable
    int getUniformLocation(const std::string& uniformName, unsigned contextId = 0);

    // returns true if a uniform name is used withing the ShaderProgram
    bool isUniformDefined(const std::string& name, unsigned contextId);

    // sets uniform variables
    void setUniform(const std::string& name, float val, unsigned contextId = 0);
    void setUniform(const std::string& name, int  val, unsigned contextId = 0);
    void setUniform(const std::string& name, bool val, unsigned contextId = 0);
    void setUniform(const std::string& name, float x, float y, float z, unsigned contextId = 0);
    void setUniform(const std::string& name, const vec2 &v, unsigned contextId = 0);
    void setUniform(const std::string& name, const vec3 &v, unsigned contextId = 0);
    void setUniform(const std::string& name, const vec4 &v, unsigned contextId = 0);
    void setUniform(const std::string& name, const mat3 &m, unsigned contextId = 0);
    void setUniform(const std::string& name, const mat4 &m, unsigned contextId = 0);
    void setUniform(const std::string& name, const Vector2& v, unsigned contextId = 0);
    void setUniform(const std::string& name, const Vector3& v, unsigned contextId = 0);
    void setUniform(const std::string& name, const Point3& v, unsigned contextId = 0);
    void setUniform(const std::string& name, const Matrix& m, unsigned contextId = 0);

	protected:

    std::map<unsigned, ShaderComponent> _components;

    // indicates an error while compiling any shader
    bool _componentCompileError;

};

} // namespace gl

} // namespace gloost


#endif // H_GLOOST_SHADERPROGRAM


