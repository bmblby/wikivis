
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


#ifndef GLOOST_SHADERCORE_H
#define GLOOST_SHADERCORE_H



// gloost system includes
#include <gloost/graph/Core.h>
#include <gloost/obsolete/Shader.h>



// cpp includes
#include <stack>



namespace gloost
{

  namespace gl { class UniformSet; class ShaderProgram;}


namespace graph
{
  class Action;


 /// Core to set a shader for the following subgraph

class ShaderCore : public Core
{
	public:

		/// class constructor with a shader
		ShaderCore(gl::ShaderProgram* shader);
		/// class constructor with a uniformSet
		ShaderCore(gl::UniformSet* uSet);
		/// class constructor with a shader and a uniformSet
		ShaderCore(gl::ShaderProgram* shader, gl::UniformSet* uSet);

		/// class destructor
		~ShaderCore();

		/* virtual */ void enterCore(const Action* action);
		/* virtual */ void exitCore (const Action* action);

    /// set the shader for this core
		void setShader(gl::ShaderProgram* shader);
    /// get the shader for this core
		const gl::ShaderProgram* getShader() const;

		void  setUniformSet(gl::UniformSet* uSet);
		gl::UniformSet* getUniformSet() const;

  private:

    gl::ShaderProgram*     _shader;
    gl::UniformSet* _uniformSet;

    /// Needed to get the shader that was set befor if a action goes up in the graph
    /// to support multiple context each context gets its own stack
    static std::vector< std::stack<gl::ShaderProgram*> > _shaderStackVector;

};


} // namespace graph
} // namespace gloost

#endif // GLOOST_SHADERCORE_H
