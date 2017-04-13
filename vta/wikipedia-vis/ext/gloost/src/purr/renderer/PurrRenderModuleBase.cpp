
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


// purr includes
#include <purr/renderer/PurrRenderModuleBase.h>
#include <purr/renderer/PurrRenderer.h>


// cpp includes
#include <string>
#include <iostream>




namespace purr
{

namespace renderer
{

/**
  \class   PurrRenderModuleBase

  \brief   Base for all purr render modules

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

PurrRenderModuleBase::PurrRenderModuleBase(const std::string&                   moduleName,
                                           const std::shared_ptr<PurrRenderer>& renderer,
                                           const std::string&                   dataDirectoryPath):
  rendermod::ModuleBase(moduleName, std::shared_ptr<rendermod::RendererBase>(renderer), dataDirectoryPath),
  _purrRenderer(renderer)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

PurrRenderModuleBase::~PurrRenderModuleBase()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the flow FlowRenderer
  \remarks ...
*/

std::shared_ptr<PurrRenderer>
PurrRenderModuleBase::getPurrRenderer()
{
 return _purrRenderer;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the renderer
  \remarks ...
*/

/*virtual*/
void
PurrRenderModuleBase::setRenderer(const std::shared_ptr<PurrRenderer>& renderer)
{
  rendermod::ModuleBase::setRenderer(renderer);
  _purrRenderer = renderer;
}


////////////////////////////////////////////////////////////////////////////////


} // namespace renderer
} // namespace purr


