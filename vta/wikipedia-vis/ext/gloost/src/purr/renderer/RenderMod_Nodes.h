
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

             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_RENDERER_RENDERMOD_NODES
#define H_PURR_RENDERER_RENDERMOD_NODES



// tnt_gloost_gui includes
#include <purr/renderer/PurrRenderModuleBase.h>

// rendermod includes
#include <rendermod/ModuleBase.h>

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gl/UniformSet.h>
#include <gloost/FreeTypeWriter.h>

// cpp includes
#include <string>
#include <vector>
#include <memory>



namespace gloost
{
  namespace gl { class Vbo;  class Ibo;  class ShaderProgram; }
  class InterleavedAttributes;
  class Matrix;
}



namespace purr
{

namespace action { class BuildMesh; }


namespace renderer
{

  class PurrRenderer;


  //  Render module for ProRenderer, renders the background texture

class RenderMod_Nodes: public PurrRenderModuleBase
{
	public:

	  static const std::string ModuleName;


    /// a std::shared_ptr of an RenderMod_Nodes instance
    typedef std::shared_ptr<RenderMod_Nodes>       shared_ptr;
    typedef std::shared_ptr<const RenderMod_Nodes> const_shared_ptr;

    // creator
    static shared_ptr create(const std::shared_ptr<PurrRenderer>& renderer,
                             const std::string&                   dataDirectoryPath);


    // class destructor
	  virtual ~RenderMod_Nodes();


    // inits the module
	  virtual void init();

    // updates the render module
	  virtual void update(const gloost::Matrix& projectionMatrix,
                        const gloost::Matrix& viewMatrix);

    // draws the render modules content
	  virtual void draw(const gloost::Matrix& projectionMatrix,
                      const gloost::Matrix& viewMatrix);


	protected:

    // class constructor
    RenderMod_Nodes(const std::shared_ptr<PurrRenderer>& renderer,
                    const std::string&  dataDirectoryPath);


	private:

	  std::shared_ptr<purr::action::BuildMesh> _gatherGeometryAction;


    std::shared_ptr<gloost::gl::ShaderProgram> _nodeShader;
    gloost::gl::UniformSet     _uniforms;
    gloost::gl::Vbo*          _nodesVbo;
    std::shared_ptr<gloost::FreeTypeWriter> _writerNodeTitle;
};


} // namespace renderer
} // namespace purr


#endif // H_PURR_RENDERER_RENDERMOD_NODES


