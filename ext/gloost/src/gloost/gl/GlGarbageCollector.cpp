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


// gloost system includes
#include <gloost/gl/GlGarbageCollector.h>
#include <gloost/TextureManager.h>

// cpp includes
#include <string>
#include <iostream>


namespace gloost
{
namespace gl
{


/**
  \class   GlGarbageCollector

  \brief   Stores gl entities to deferre their destruction until a defined gl state is guaranteed

  \author  Felix Weiszig
  \date    Febuary 2016
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////

/*static*/ GlGarbageCollector::shared_ptr GlGarbageCollector::_instance = nullptr;

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

GlGarbageCollector::GlGarbageCollector():
  _mutex(),
  _fbos(),
  _gBuffers(),
  _multiGlEntities(),
  _textureGids()
{
  // insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
GlGarbageCollector::~GlGarbageCollector()
{
  // insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the GlGarbageCollector
  \param   ...
  \remarks ...
*/

/*static*/
const std::shared_ptr<GlGarbageCollector>&
GlGarbageCollector::get()
{
  if (!_instance)
  {
    _instance = std::shared_ptr<GlGarbageCollector>(new GlGarbageCollector());
  }
  return _instance;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds an gl::Fbo to dispose of
  \remarks ...
*/

/*static*/
void
GlGarbageCollector::put(const std::shared_ptr<Fbo>& fbo)
{
  auto garbage = get();
  auto lock = garbage->getLockGuard();
  garbage->_fbos.insert(fbo);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds an gl::GBuffer to dispose of
  \remarks ...
*/

/*static*/
void
GlGarbageCollector::put(const std::shared_ptr<GBuffer>& gBuffer)
{
  auto garbage = get();
  auto lock = garbage->getLockGuard();
  garbage->_gBuffers.insert(gBuffer);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds an gl::MultiGlContextEntity like gl::Ibo,  gl::Vbo,  gl::Tbo, gl::Texture, ... to dispose of
  \remarks ...
*/

/*static*/
void
GlGarbageCollector::put(const std::shared_ptr<MultiGlContextEntity>& glEntity)
{
  auto garbage = get();
  auto lock = garbage->getLockGuard();
  garbage->_multiGlEntities.insert(glEntity);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds an texture gid to dispose of
  \remarks ...
*/

/*static*/
void
GlGarbageCollector::putTextureGid(gloostId textureGid)
{
  auto garbage = get();
  auto lock = garbage->getLockGuard();
  gloost::TextureManager::get()->getTextureReference(textureGid);
  garbage->_textureGids.insert(textureGid);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   clears all entities from the garbage (requires gl context)
  \remarks ...
*/

/*static*/
void
GlGarbageCollector::flush()
{
  const auto garbage = get();
  const auto lock = garbage->getLockGuard();
  garbage->_fbos.clear();
  garbage->_gBuffers.clear();
  garbage->_multiGlEntities.clear();

  auto textureManager = gloost::TextureManager::get();
  for (const auto& textureGid : garbage->_textureGids)
  {
    textureManager->dropReference(textureGid);
//    textureManager->removeTexture(textureGid);
  }
//  textureManager->cleanUp();
//  std::cerr << std::endl << "textureManager->getSize(): " << textureManager->getSize();
//  std::cerr << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   locks the node and returns a lock_guard instance
  \remarks ...
*/

/*virtual */
std::shared_ptr<std::lock_guard<std::mutex> >
GlGarbageCollector::getLockGuard() const
{
  return std::shared_ptr<std::lock_guard<std::mutex> >(new std::lock_guard<std::mutex>(_mutex));
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gl
} // namespace gloost

