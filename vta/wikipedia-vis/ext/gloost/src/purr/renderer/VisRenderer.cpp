
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


// libpurr includes
#include <purr/renderer/PurrRenderer.h>
#include <purr/renderer/RenderMod_Nodes.h>
//#include <purr/renderer/RenderMod_VisNodes.h>
#include <purr/model/PurrModel.h>

// librendermod includes
#include <rendermod/ModuleBase.h>


// gloost system includes
#include <gloost/gl/Fbo.h>
#include <gloost/gl/GBuffer.h>

// cpp includes
#include <string>
#include <iostream>

namespace purr
{

namespace renderer
{

/**
  \class   PurrRenderer

  \brief   Renderer using current OpenGL standard

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a PurrRenderer
  \remarks ...
*/

/*static*/
PurrRenderer::shared_ptr
PurrRenderer::create(const std::string& dataDirectoryPath,
                     std::shared_ptr<model::PurrModel> model)
{
	shared_ptr renderer(new PurrRenderer(dataDirectoryPath, model));
	return renderer;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

PurrRenderer::PurrRenderer(const std::string& dataDirectoryPath,
                           std::shared_ptr<model::PurrModel> model):
  rendermod::RendererBase(dataDirectoryPath),
  _model(model)
//  _startNode(nullptr)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

PurrRenderer::~PurrRenderer()
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   inits the render modules and adds them to the renderer
  \param   ...
  \remarks ...
*/

/*virtual*/
void
PurrRenderer::initModules()
{
  // module to render Nodes
  auto mod_nodes    = RenderMod_Nodes::create(shared_from_this(), getDataDirectoryPath());
  addModule(mod_nodes);
//  auto mod_visNodes = RenderMod_VisNodes::create(shared_from_this(), getDataDirectoryPath());
//  addModule(mod_visNodes);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   resizes the screen
  \param   ...
  \remarks ...
*/

/*virtual*/
void
PurrRenderer::resize(unsigned screenWidth, unsigned screenHeight)
{
  _screenWidth  = screenWidth;
  _screenHeight = screenHeight;

  _gBuffer = nullptr;
  _gBuffer = gloost::gl::GBuffer::create(screenWidth, screenHeight, GL_RGBA);
  _gBuffer->addTexture();

  glClearColor(1.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _gBuffer->createDepthBuffer();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
PurrRenderer::updateModules()
{
  if (!_model)
  {
    std::cerr << std::endl << "PurrRenderer::updateModules(): MODEL IS NULLPTR !!!";
  }

  for (const auto& module : getRenderModules())
  {
    module->update(_projectionMatrix, _viewMatrix);
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
PurrRenderer::draw()
{
  if (!_gBuffer)
  {
    std::cerr << std::endl << "           ";
    std::cerr << std::endl << "(D'OH) PurrRenderer::draw(): ";
    std::cerr << std::endl << "       I have NO GBuffer!";
    std::cerr << std::endl << "       Call resize(unsigned screenWidth, unsigned screenHeight) once to init one.";
    return;
  }

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {
    _gBuffer->bind({GL_COLOR_ATTACHMENT0});
    _gBuffer->getFbo()->bind();

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);
      glViewport(0, 0, _screenWidth, _screenHeight);

      int viewportWidth  = _screenWidth;
      int viewportHeight = viewportWidth / (_screenWidth/(float)_screenHeight);

      gloost::gl::gloostOrtho(_projectionMatrix,
                          0.0, viewportWidth,
                          0.0, viewportHeight,
                          0.0,
                          100.0);

      // TODO: MOVE THE MATRICES TO THE MODEL
//      int zoom = gloost::math::max(1, (int)_model->getCameraZoom());
      int zoom = 1;
      _viewMatrix = gloost::Matrix::createIdentity();
      _viewMatrix = _viewMatrix * gloost::Matrix::createTransMatrix({viewportWidth*0.5, 0.0, 0.0});
//      _viewMatrix = _viewMatrix * gloost::Matrix::createTransMatrix(_model->getCameraPosition().int_copy() * zoom);
      _viewMatrix = _viewMatrix * gloost::Matrix::createTransMatrix(0.0,0.0,0.0);
      _viewMatrix = _viewMatrix * gloost::Matrix::createScaleMatrix(zoom);
      _viewMatrix = _viewMatrix * gloost::Matrix::createTransMatrix({-viewportWidth*0.5, 0.0, 0.0});

      for (const auto& module : getRenderModules())
      {
        module->draw(_projectionMatrix, _viewMatrix);
      }
    }
    _gBuffer->unbind();

  }
  glPopAttrib();

}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   triggers shader reload in all render modules
  \param   ...
  \remarks ...
*/

/*virtual*/
void
PurrRenderer::reloadShaders()
{
  std::cerr << std::endl << "Message from PurrRenderer::reloadShaders()";
  std::cerr << std::endl << "             Reloading shaders of all render modules";

  for (const auto& module : getRenderModules())
  {
    module->reloadShaders();
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the model::PurrModel this renderer feeds from
  \param   ...
  \remarks ...
*/

/*virtual*/
std::shared_ptr<model::PurrModel>
PurrRenderer::getModel()
{
  return _model;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace renderer
} // namespace purr
