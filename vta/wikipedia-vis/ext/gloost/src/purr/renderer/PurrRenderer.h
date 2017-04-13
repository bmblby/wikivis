
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

#ifndef H_PURR_RENDERER_PURRRENDERER
#define H_PURR_RENDERER_PURRRENDERER

// librendermod includes
#include <rendermod/RendererBase.h>

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <memory>

namespace gloost
{
  class TextureText;
}

namespace purr
{

namespace model { class Node; class PurrModel; }

namespace renderer
{

  //  Renderer using current OpenGL standard

class PurrRenderer: public rendermod::RendererBase,
                    public std::enable_shared_from_this<PurrRenderer>
{
	public:

    /// a std::shared_ptr of an PurrRenderer instance
    typedef std::shared_ptr<PurrRenderer>       shared_ptr;
    typedef std::shared_ptr<const PurrRenderer> const_shared_ptr;


    // class factory
    static shared_ptr create(const std::string& dataDirectoryPath,
                             std::shared_ptr<model::PurrModel> purrModel);

    // class destructor
	  virtual ~PurrRenderer();

    // resizes the screen
    virtual void resize(unsigned screenWidth, unsigned screenHeight);

    // inits the render modules and adds them to the renderer
    virtual void initModules();

    // updtaes geometry from world to renderer for all modules
    virtual void updateModules();

    virtual void draw();

    // triggers shader reload in all render modules
    virtual void reloadShaders();

    // returns the GraphicalModel this renderer feeds from
    std::shared_ptr<model::PurrModel> getModel();

	protected:

    // class constructor
    PurrRenderer(const std::string& dataDirectoryPath,
                 std::shared_ptr<model::PurrModel> purrModel);

	private:

    std::shared_ptr<model::PurrModel> _model;

};

} // namespace renderer
} // namespace purr


#endif // H_PURR_RENDERER_PURRRENDERER
