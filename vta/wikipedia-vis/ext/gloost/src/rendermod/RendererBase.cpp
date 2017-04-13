
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

// rendermod includes
#include <rendermod/RendererBase.h>


// gloost includes
#include <gloost/gl/GBuffer.h>


// cpp includes
#include <string>
#include <iostream>



namespace rendermod
{


/**
  \class   RendererBase

  \brief   ...

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    June 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

RendererBase::RendererBase(const std::string& dataDirectoryPath):
  _dataDirectoryPath(dataDirectoryPath),
  _screenWidth(1),
  _screenHeight(1),
  _gBuffer(nullptr),
  _projectionMatrix(gloost::Matrix::createIdentity()),
  _viewMatrix(gloost::Matrix::createIdentity()),
  _modelMatrix(gloost::Matrix::createIdentity()),
  _renderModules()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

RendererBase::~RendererBase()
{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   this should be put to a View class
  \param   ...
  \remarks ...
*/


void
RendererBase::viewUpdate()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief    adds a rendermodule
  \param   ...
  \remarks ...
*/


void
RendererBase::addModule(std::shared_ptr<ModuleBase> module)
{
  _renderModules.push_back(module);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   resizes the screen
  \param   ...
  \remarks ...
*/

/*virtual*/
void
RendererBase::resize(unsigned screenWidth, unsigned screenHeight)
{
  _screenWidth  = screenWidth;
  _screenHeight = screenHeight;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the screen width
  \param   ...
  \remarks ...
*/


unsigned
RendererBase::getScreenWidth() const
{
  return _screenWidth;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the screen height
  \param   ...
  \remarks ...
*/


unsigned
RendererBase::getScreenHeight() const
{
  return _screenHeight;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the gloostId of the color buffer or 0
  \param   ...
  \remarks ...
*/

gloost::gloostId
RendererBase::getColorBufferGid() const
{
  if (!_gBuffer || !_gBuffer->getTextureGids().size())
  {
    return 0;
  }
  return _gBuffer->getTextureGids()[0];
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the projection matrix
  \param   ...
  \remarks ...
*/


const gloost::Matrix&
RendererBase::getProjectionMatrix() const
{
  return _projectionMatrix;;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the view matrix
  \param   ...
  \remarks ...
*/


const gloost::Matrix&
RendererBase::getViewMatrix() const
{
  return _viewMatrix;;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the model matrix
  \param   ...
  \remarks ...
*/


const gloost::Matrix&
RendererBase::getModelMatrix() const
{
  return _modelMatrix;;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a matrix, transforming from screen to world cords
  \param   ...
  \remarks ...
*/


gloost::Matrix
RendererBase::getScreenToWorldMatrix() const
{
  return gloost::Matrix::createIdentity();
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the GBuffer of this renderer
  \param   ...
  \remarks ...
*/


std::shared_ptr<gloost::gl::GBuffer>
RendererBase::getGBuffer()
{
  return _gBuffer;
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the path to the ressources needed for the renderer
  \param   ...
  \remarks ...
*/


const std::string&
RendererBase::getDataDirectoryPath() const
{
  return _dataDirectoryPath;
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a reference to the container of base render module instances
  \param   ...
  \remarks ...
*/


RendererBase::BaseModuleContainer&
RendererBase::getRenderModules()
{
  return _renderModules;
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a reference to the container of base render module instances
  \param   ...
  \remarks ...
*/


const RendererBase::BaseModuleContainer&
RendererBase::getRenderModules() const
{
  return _renderModules;
}


///////////////////////////////////////////////////////////////////////////////



} // namespace rendermod


