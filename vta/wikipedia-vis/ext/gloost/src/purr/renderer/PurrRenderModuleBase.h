
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


#ifndef H_PURR_RENDERER_PURRRENDERMODULEBASE
#define H_PURR_RENDERER_PURRRENDERMODULEBASE



// rendermod includes
#include <rendermod/ModuleBase.h>

// gloost system includes
#include <gloost/gloostConfig.h>

// cpp includes
#include <string>
#include <vector>
#include <memory>



namespace purr
{

namespace renderer
{

  class PurrRenderer;


  //  Base for all purr render modules

class PurrRenderModuleBase: public rendermod::ModuleBase
{
	public:

    // class destructor
	  virtual ~PurrRenderModuleBase();


	  // returns the purr FlowRenderer
	  std::shared_ptr<PurrRenderer> getPurrRenderer();


	  // sets the renderer
	  virtual void setRenderer(const std::shared_ptr<PurrRenderer>& renderer);



	protected:

    // class constructor
    PurrRenderModuleBase(const std::string&                   moduleName,
                         const std::shared_ptr<PurrRenderer>& renderer,
                         const std::string&                   dataDirectoryPath);


	private:

    std::shared_ptr<PurrRenderer> _purrRenderer;

};


} // namespace renderer
} // namespace purr


#endif // H_PURR_RENDERER_PURRRENDERMODULEBASE


