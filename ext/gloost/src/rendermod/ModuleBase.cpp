
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

// tnt_gloost_gui includes
#include <rendermod/ModuleBase.h>
#include <rendermod/RendererBase.h>

// gloost system includes
#include <gloost/gl/ShaderProgram.h>



// cpp includes
#include <string>
#include <iostream>



namespace rendermod
{

/**
  \class   ModuleBase

  \brief   Base module

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    july 2012
  \remarks base class of all render modules
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

ModuleBase::ModuleBase(const std::string&                   moduleName,
                       const std::shared_ptr<RendererBase>& renderer,
                       const std::string&                   dataDirectoryPath):
    _moduleName(moduleName),
    _shaders(),
    _renderer(renderer),
    _dataDirectoryPath(dataDirectoryPath)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

ModuleBase::~ModuleBase()
{
//
//  std::cerr << std::endl;
//  std::cerr << std::endl << "ModuleBase::~ModuleBase(): " << this;
//  std::cerr << std::endl;

//  auto pos = _shaders.begin();
//  while (pos!=_shaders.end())
//  {
//    auto* shader = (*pos);
//
//
//    pos = _shaders.erase(pos);
//    delete shader;
////    ++pos;
//  }
//  _shaders.clear();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   triggers reload of all shaders of a module
  \param   ...
  \remarks ...
*/

void
ModuleBase::reloadShaders()
{
  std::cerr << std::endl << "Reloading " << _shaders.size() << " shader(s) for " << _moduleName;

  for (unsigned i=0; i!=_shaders.size(); ++i)
  {
    _shaders[i]->reloadShaders();
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the renderer
  \param   ...
  \remarks ...
*/

std::shared_ptr<RendererBase>
ModuleBase::getRenderer()
{
  return _renderer.lock();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the renderer
  \remarks ...
*/

/*virtual*/
void
ModuleBase::setRenderer(const std::shared_ptr<RendererBase>& renderer)
{
  _renderer = renderer;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns true if this instance has a renderer set
  \remarks ...
*/

/*virtual*/
bool
ModuleBase::hasRenderer()
{
  return (bool)getRenderer();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the path to the ressource directory of this module
  \param   ...
  \remarks ...
*/

const std::string&
ModuleBase::getRessourcePath() const
{
  return _dataDirectoryPath;
}


////////////////////////////////////////////////////////////////////////////////


} // namespace rendermod


