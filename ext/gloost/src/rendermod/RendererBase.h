
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

#ifndef H_LIBRENDERMOD_RENDERERBASE
#define H_LIBRENDERMOD_RENDERERBASE



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/gl/gloostGlUtil.h>


// cpp includes
#include <string>
#include <memory>


namespace gloost
{
  namespace gl { class GBuffer; }
}



namespace rendermod
{


  class ModuleBase;



  //  RendererBase template

class RendererBase
{
	public:

	  typedef std::vector<std::shared_ptr<rendermod::ModuleBase> > BaseModuleContainer;


    // class destructor
	  virtual ~RendererBase();


    // adds a rendermodule
    void addModule(std::shared_ptr<ModuleBase> module);

    // updtaes geometry from world to renderer for all modules
    virtual void updateModules() = 0;

    // performs a draw
    virtual void draw() = 0;

    // this should be put to a View class
    void viewUpdate();


    // resizes the screen
    virtual void resize(unsigned screenWidth, unsigned screenHeight);

    // returns the screen width
    unsigned getScreenWidth() const;

    // returns the screen height
    unsigned getScreenHeight() const;

    // returns the gloostId of the color buffer or 0
    gloost::gloostId getColorBufferGid() const;

    // returns the GBuffer of this renderer
    std::shared_ptr<gloost::gl::GBuffer> getGBuffer();



    // returns the projection matrix
    const gloost::Matrix& getProjectionMatrix() const;

    // returns the view matrix
    const gloost::Matrix& getViewMatrix() const;

    // returns the model matrix
    const gloost::Matrix& getModelMatrix() const;

    // returns a matrix transforming from screen to world ccords
    gloost::Matrix getScreenToWorldMatrix() const;



    // triggers shader reload in all render modules
    virtual void reloadShaders() = 0;


    // returns the path to the ressources needed for the renderer
    const std::string& getDataDirectoryPath() const;


    // returns a reference to the container of base render module instances
    BaseModuleContainer& getRenderModules();
    const BaseModuleContainer& getRenderModules() const;

	protected:

    std::string _dataDirectoryPath;


    unsigned _screenWidth;
    unsigned _screenHeight;


    std::shared_ptr<gloost::gl::GBuffer> _gBuffer;


    // ...
    gloost::Matrix _projectionMatrix;
    gloost::Matrix _viewMatrix;
    gloost::Matrix _modelMatrix;

    // class constructor
    RendererBase(const std::string& dataDirectoryPath);

	private:

    BaseModuleContainer _renderModules;

};


} // namespace rendermod


#endif // H_LIBRENDERMOD_RENDERERBASE


