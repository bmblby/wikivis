
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

#ifndef H_LIBRENDERMOD_MODULEBASE
#define H_LIBRENDERMOD_MODULEBASE

// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <vector>
#include <memory>

namespace gloost
{
  namespace gl { class ShaderProgram; }
  class Matrix;
  class MouseState;
}

namespace rendermod
{
  class RendererBase;

  //  base class of all render modules

class ModuleBase
{
	public:

    /// a std::shared_ptr of an ModuleBase instance
    typedef std::shared_ptr<ModuleBase>       shared_ptr;
    typedef std::shared_ptr<const ModuleBase> const_shared_ptr;

    // class destructor
	  virtual ~ModuleBase();

    // inits the module
	  virtual void init(){};

    // updates the render module
	  virtual void update(const gloost::Matrix& projectionMatrix,
                        const gloost::Matrix& viewMatrix) = 0;

    // draws the render modules content
	  virtual void draw(const gloost::Matrix& projectionMatrix,
                      const gloost::Matrix& viewMatrix){};

    // triggers shader reload
    void reloadShaders();

    // returns the path to the ressource directory of this module
    virtual const std::string& getRessourcePath() const;

	  // sets the renderer
	  virtual void setRenderer(const std::shared_ptr<RendererBase>& renderer);

	  // returns true if this instance has a renderer set
	  bool hasRenderer();

	protected:

    // class constructor
    ModuleBase(const std::string&                   moduleName,
               const std::shared_ptr<RendererBase>& renderer,
               const std::string&                   dataDirectoryPath);

    // returns the renderer
    std::shared_ptr<RendererBase> getRenderer();

	  // name of the module just for the log
	  std::string _moduleName;

    // vector of shaders used to reload them all
	  std::vector<std::shared_ptr<gloost::gl::ShaderProgram>> _shaders;

	private:

	  // renderer
	  std::weak_ptr<RendererBase>   _renderer;
	  std::string                   _dataDirectoryPath;
};

} // namespace rendermod

#endif // H_LIBRENDERMOD_MODULEBASE
