
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



#ifndef GLOOST_GL_UNIFORMSET_H
#define GLOOST_GL_UNIFORMSET_H



// gloost system includes
#include <gloost/glErrorUtil.h>
#include <gloost/Material.h>



// cpp includes
#include <string>
#include <map>



namespace gloost
{
namespace gl
{
  class ShaderProgram;


 ///  Set of GLSL shader uniform variables to configure a shader with

class UniformSet: public Material
{
	public:
    /// class constructor
    UniformSet();
    /// class destructor
    ~UniformSet();

    /// set uniforms to shader. uniform names will have prefix+name as name in the shader
    void applyToShader(const unsigned int shaderHandle, unsigned contextId = 0u);

    /// set uniforms to shader. uniform names will have prefix+name as name in the shader
    void applyToShader(ShaderProgram* shader, unsigned contextId = 0u);
    void applyToShader(const std::shared_ptr<ShaderProgram>& shader, unsigned contextId = 0u);

    /// set uniforms to shader. This uniforms will accumulate to existing uniforms in the state
    void addToShader(const unsigned int shaderHandle, unsigned contextId = 0u);

    /// set uniforms to shader. This uniforms will accumulate to existing uniforms in the state
    void addToShader(ShaderProgram* shader, unsigned contextId = 0u);

	private:

    // TODO: NOT MULTIGL CONTEXT COMPATIBLE
    static unsigned int _current_texture_offset;

  protected:

    ///


};


} // namespace gl
} // namespace gloost


#endif // GLOOST_GL_UNIFORMSET_H


