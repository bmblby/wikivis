
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

// tnt_gloost_gui includes
#include <purr/renderer/RenderMod_Nodes.h>
#include <purr/renderer/PurrRenderer.h>
#include <purr/model/PurrModel.h>
#include <purr/model/Node.h>
#include <purr/action/BuildMesh.h>

// gloost system includes
#include <gloost/TextureManager.h>
#include <gloost/InterleavedAttributes.h>
#include <gloost/gl/Vbo.h>
#include <gloost/gl/ShaderProgram.h>
#include <gloost/Vector3.h>
//#include <gloost/util/NineFace.h>
#include <gloost/util/Timer.h>
#include <gloost/postal/common.h>

// cpp includes
#include <string>
#include <iostream>
#include <omp.h>
#include <memory.h>

namespace purr
{

namespace renderer
{

/**
  \class   RenderMod_Nodes

  \brief   Render module for ProRenderer, renders the background texture

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    july 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string RenderMod_Nodes::ModuleName = std::string("purr::renderer::RenderMod_Nodes");

////////////////////////////////////////////////////////////////////////////////

/**
  \brief
  \remarks ...
*/

/*static*/
RenderMod_Nodes::shared_ptr
RenderMod_Nodes::create(const std::shared_ptr<PurrRenderer>& renderer,
                        const std::string&  dataDirectoryPath)
{
	shared_ptr instance(new RenderMod_Nodes(renderer, dataDirectoryPath));
	return instance;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

RenderMod_Nodes::RenderMod_Nodes(const std::shared_ptr<PurrRenderer>& renderer,
                                 const std::string&  dataDirectoryPath):
  PurrRenderModuleBase("RenderMod_Nodes", renderer, dataDirectoryPath),
  _gatherGeometryAction(nullptr),
  _nodeShader(nullptr),
  _uniforms(),
  _nodesVbo(nullptr),
  _writerNodeTitle(nullptr)
{
	_nodeShader = gloost::gl::ShaderProgram::create();
	_nodeShader->attachShaderFile(GLOOST_SHADERPROGRAM_VERTEX_SHADER,
                            getRessourcePath() + "/shader/libpurr/RenderMod_Nodes/2d_texture.vs");
	_nodeShader->attachShaderFile(GLOOST_SHADERPROGRAM_FRAGMENT_SHADER,
                            getRessourcePath() + "/shader/libpurr/RenderMod_Nodes/2d_texture.fs");

  _shaders.push_back(_nodeShader);

//  gloost::gl::Texture* ninefaceTexture = new gloost::gl::Texture(getRessourcePath() + "nineface/tnt_extra_6.png");
//  _ninefaceTextureGid = gloost::TextureManager::get()->addTexture(ninefaceTexture);

//  ninefaceTexture->enableMipmaps();
//  ninefaceTexture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP);
//  ninefaceTexture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP);
//  ninefaceTexture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  ninefaceTexture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  gloost::gl::Texture* radialTexture = new gloost::gl::Texture(dataDirectoryPath + "/texture/libpurr/radial_gradient.png");
  _uniforms.set_sampler("u_radialMap", gloost::TextureManager::get()->addTexture(radialTexture));


  gloost::InterleavedAttributes::shared_ptr vertexAttribs = gloost::InterleavedAttributes::create();
  vertexAttribs->addAttribute(2, "in_position");
  vertexAttribs->addAttribute(1, "in_nodeDepth");
  vertexAttribs->addAttribute(2, "in_texcoords");

  _nodesVbo = new gloost::gl::Vbo(vertexAttribs);

  _writerNodeTitle = std::shared_ptr<gloost::FreeTypeWriter>( new gloost::FreeTypeWriter(getRessourcePath() + "/font/shared/DejaVuSans/DejaVuSansCondensed.ttf",
                                                              11,
                                                              true));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

RenderMod_Nodes::~RenderMod_Nodes()
{
//	gloost::TextureManager::get()->removeTexture(_ninefaceTextureGid);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   inits the module
  \param   ...
  \remarks ...
*/

void
RenderMod_Nodes::init()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   updates the render module
  \param   ...
  \remarks ...
*/

void
RenderMod_Nodes::update(const gloost::Matrix& projectionMatrix,
                        const gloost::Matrix& viewMatrix)
{

//  std::cerr << std::endl << "From RenderMod_Nodes::update(): " << this;
//  std::cerr << std::endl << "     MESSAGE";


//  if (!_nodesVbo->getVertexAttributes()->getNumPackages())
  {
    static gloost::util::Timer timer;
    timer.start();

    auto newAttribs = gloost::InterleavedAttributes::create(_nodesVbo->getVertexAttributes()->getLayout(), 0);
    _nodesVbo->setVertexAttributes(newAttribs);

    // start an action to collect all nodes holding a behavior::DrawableEntity instance
    _gatherGeometryAction = purr::action::BuildMesh::create();
    _gatherGeometryAction->begin(getPurrRenderer()->getModel()->getRootNode());

    auto& geometryList = _gatherGeometryAction->getGeometryList();

    // allocate 4 packages for each node (since it's visualized as rectangle)
    newAttribs->resize(geometryList.size()*4u);
    auto& vertexAttribContainer = newAttribs->getVector();

    // build a world space quad for each node
    for (unsigned i=0u; i!=geometryList.size(); ++i)
    {
      unsigned packageIndex = newAttribs->getPackageIndex(i*4u);

      const auto& entry = geometryList[i];

      const auto& worldSpacePMin = entry->_worldSpaceAabb.getPMin();
      const auto& worldSpacePMax = entry->_worldSpaceAabb.getPMax();

      // 4x position for a quad (cw)
      vertexAttribContainer[packageIndex++] = worldSpacePMin[0];
      vertexAttribContainer[packageIndex++] = worldSpacePMin[1];
      vertexAttribContainer[packageIndex++] = entry->_nodeDepth;
      vertexAttribContainer[packageIndex++] = 0.0;
      vertexAttribContainer[packageIndex++] = 0.0;

      vertexAttribContainer[packageIndex++] = worldSpacePMin[0];
      vertexAttribContainer[packageIndex++] = worldSpacePMax[1];
      vertexAttribContainer[packageIndex++] = entry->_nodeDepth;
      vertexAttribContainer[packageIndex++] = 0.0;
      vertexAttribContainer[packageIndex++] = 1.0;

      vertexAttribContainer[packageIndex++] = worldSpacePMax[0];
      vertexAttribContainer[packageIndex++] = worldSpacePMax[1];
      vertexAttribContainer[packageIndex++] = entry->_nodeDepth;
      vertexAttribContainer[packageIndex++] = 1.0;
      vertexAttribContainer[packageIndex++] = 1.0;

      vertexAttribContainer[packageIndex++] = worldSpacePMax[0];
      vertexAttribContainer[packageIndex++] = worldSpacePMin[1];
      vertexAttribContainer[packageIndex++] = entry->_nodeDepth;
      vertexAttribContainer[packageIndex++] = 1.0;
      vertexAttribContainer[packageIndex++] = 0.0;
    }
    timer.stop();
    _nodesVbo->removeFromContext();
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   draws the render modules content
  \param   ...
  \remarks ...
*/

void
RenderMod_Nodes::draw(const gloost::Matrix& projectionMatrix,
                      const gloost::Matrix& viewMatrix)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

//  glColor4f(1.0,1.0,1.0,1.0);

//  gloost::Matrix myModelMatrix = gloost::Matrix::createTransMatrix(node->getPosition());

  unsigned itemIndex = 0u;
  for (const auto& item : _gatherGeometryAction->getGeometryList())
  {
    // set state
    _nodeShader->use();
    _uniforms.applyToShader(_nodeShader->getHandle());
    _nodesVbo->bind();

    _nodeShader->setUniform("u_mvpMatrix",  projectionMatrix*viewMatrix);
    _nodeShader->setUniform("u_activation", item->_activation);

    // draw
    _nodeShader->setUniform("u_color",      gloost::vec4(.56f));
    _nodesVbo->draw(GL_QUADS, itemIndex*4u, 4u);

    // draw
    _nodeShader->setUniform("u_color",      gloost::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    _nodesVbo->draw(GL_LINE_LOOP, itemIndex*4u, 4u);

    // remove state
    _nodesVbo->unbind();
    _nodeShader->disable();

    // text
    if (item->_node->getLabel().length())
    {
      gloost::Point3 nodeOffset = item->_worldSpaceAabb.getPMin();

      _writerNodeTitle->beginText(projectionMatrix*viewMatrix);
      {
        auto labelOffset = item->_drawableEntityBehavior->getLabelOffset();
        static const std::vector<gloost::vec4> titleColors({gloost::vec4(0.0,0.0,0.0,0.5f),
                                                            gloost::vec4(1.0f,1.0f,1.0f,1.0f)});

        _writerNodeTitle->setAlignModeH(GLOOST_FREETYPEWRITER_ALIGN_LEFT);
        for (int i=0u; i!=2; ++i)
        {
          // title
          _writerNodeTitle->setTintColor(titleColors[i]);
          _writerNodeTitle->writeLine(int(nodeOffset[0]) - i + int(labelOffset[0]) ,
                                      int(nodeOffset[1]) + i + int(labelOffset[1]),
                                      item->_node->getLabel());
        }
      }
      _writerNodeTitle->endText();
    }
    ++itemIndex;
  }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace renderer
} // namespace purr
