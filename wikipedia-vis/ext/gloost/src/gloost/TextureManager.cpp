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
#include <gloost/TextureManager.h>

// cpp includes
#include <iostream>
#include <vector>

namespace gloost
{
/**
 \class TextureManager
 \brief Texture container and factory. Use this class to create, manage and share your Texture
         resources within your application.
 \author Felix Weiszig
 \date   December 2009
 \remarks Consult the step_04_Textures_and_TextureManager
           tutorial located in <i><gloost>/tutorials/essentials/</i> to learn more.
           <br>This class is a singleton. To get a reference to the instance write:
           \code
           gloost::TextureManager* texManager = gloost::TextureManager::getInstance();
           \endcode
           To load a texture write:
           \code
           // you can do it like this
           gloost::gloostId texId = texManager->createTexture("Image.png");
           // or like this
           gloost::gloostId texId = gloost::TextureManager::getInstance()->createTexture("Image.png");
           \endcode
           To get the Texture instance for the gloostId try:
           \code
           // This will increment the reference counter
           gloost::Texture* tex = texManager->getTextureReference(texId);
           // This will NOT increment the reference counter
           gloost::Texture* tex = texManager->getTextureWithoutRefcount(texId);
           \endcode
*/

/*static*/  TextureManager* TextureManager::_theinstance = 0;

///////////////////////////////////////////////////////////////////////////////

/**
 \brief Class constructor
*/

TextureManager::TextureManager():
  _textures(),
  _shareMap(),
  _idMutex(),
  _addRemoveMutex()
{
//  // create a default texture at id 0
//  Texture* defaultTexture = new Texture(512,
//                                        512,
//                                        16,
//                                        GL_TEXTURE_2D,
//                                        GL_RGBA,
//                                        GL_RGBA,
//                                        GL_FLOAT);
//
//  addTexture(defaultTexture);
}

///////////////////////////////////////////////////////////////////////////////

/// class destructor

TextureManager::~TextureManager()
{
  getTextureWithoutRefcount(0)->dropReference();
  cleanUp();
}

///////////////////////////////////////////////////////////////////////////////

/// get an instance to the TextureManager

TextureManager*
TextureManager::getInstance()
{
  if(!_theinstance)
  {
    _theinstance = new TextureManager();
  }
  return _theinstance;
}

///////////////////////////////////////////////////////////////////////////////

/// get an instance to the TextureManager

TextureManager*
TextureManager::get()
{
  if(!_theinstance)
  {
    _theinstance = new TextureManager();
  }
  return _theinstance;
}

//////////////////////////////////////////////////////////////////////////////////////////

/// returns a new texture id (thread save)

gloostId
TextureManager::getNewTextureGid()
{
  boost::mutex::scoped_lock lock(_idMutex);
  static gloostId idCounter = 0;
  ++idCounter;
  return idCounter;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
  Creates empty Texture with specific id object of given width and height
  This will overwrite existing textures with this id, so don't mess around with this
  (refCount +)
**/


gloostId
TextureManager::createTexture(gloostId gid, unsigned width, unsigned height)
{
  boost::mutex::scoped_lock lock(_addRemoveMutex);

  // generate a new texture
  _textures[gid] = new gl::Texture( width,
      height,
      16,
      GL_TEXTURE_2D,
      GL_RGBA,
      GL_RGBA,
      GL_FLOAT);
  // increment the refCounter +1
  _textures[gid]->takeReference();
  return gid;
}

//////////////////////////////////////////////////////////////////////////////////////////

/// load a texture (automatic configuration of the GL_Texture_2Ds format and type) (tested: jpg, png24, png32)

gloostId
TextureManager::createTexture(std::string file_name, bool share)
{
  return createTexture(gloost::PathType(file_name), share);
}

//////////////////////////////////////////////////////////////////////////////////////////

/// load a texture (automatic configuration of the GL_Texture_2Ds format and type) (tested: jpg, png24, png32)

gloostId
TextureManager::createTexture(gloost::PathType file_name, bool share)
{
  boost::mutex::scoped_lock lock(_addRemoveMutex);

  // if this texture is shared we first look in the map if this texture
  // has been allready loaded
  if (share)
  {
    auto pos = _shareMap.find(file_name);
    if (pos != _shareMap.end())
    {
      /// increment the reference counter
      getTextureReference((*pos).second);
      return (*pos).second;
    }
  }
  // we take us a new unique id...
  gloostId newId = getNewTextureGid();
  // generate a new texture
  _textures[newId] = new gl::Texture(file_name);
  // increment the refCounter +1
  _textures[newId]->takeReference();
  // add the texture to the share lookup map
  if (share)
  {
    _textures[newId]->setShared(true);
    _shareMap[file_name] = newId;
  }
  return newId;
}

//////////////////////////////////////////////////////////////////////////////////////////

/// Add existing Texture to be managed by the TextureManager (refCount +)

gloostId
TextureManager::addTexture(gl::Texture* texture)
{
  boost::mutex::scoped_lock lock(_addRemoveMutex);
  gloostId id = getNewTextureGid();
  texture->takeReference();
  _textures[id] = texture;
  return id;
}

//////////////////////////////////////////////////////////////////////////////////////////

/// removes a texture from the TextureManager

void
TextureManager::removeTexture(gloostId gid)
{
  boost::mutex::scoped_lock lock(_addRemoveMutex);

  auto pos = _textures.find(gid);

  if (pos != _textures.end())
  {
    gl::Texture* texture = (*pos).second;
    _textures.erase(pos);

    if (texture->isShared())
    {
      texture->setShared(false);
      _shareMap.erase(texture->getFilePath());
    }
    texture->dropReference();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

/// returns reference to gloost::Texture object and increments the reference counter (refCount +), 0 if id was not found

gl::Texture*
TextureManager::getTextureReference(gloostId gid)
{
  auto pos = _textures.find(gid);
  if (pos != _textures.end())
  {
    // (refCount +)
    (*pos).second->takeReference();
    return (*pos).second;
  }
//#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
//  std::cout<< std::endl;
//  std::cout<< std::endl << "Warning from TextureManager::getTextureReference():";
//  std::cout<< std::endl << "             Could not find existing texture with gid = " << gid << " !";
//  std::cout<< std::endl << "             Returning 0";
//  std::flush(std::cout);
//#endif
  /*
    This is buggy! If the id is bigger than _idCounter...
  */
//
//  _textures[0]->takeReference();
//  return _textures[0];
//  _textures[0]->takeReference();
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////

/// returns reference to gloost::Texture object WITHOUT increments the reference counter, 0 if id was not found

gl::Texture*
TextureManager::getTextureWithoutRefcount(gloostId gid)
{
  auto pos = _textures.find(gid);
  if (pos != _textures.end())
  {
    return (*pos).second;
  }
//
//#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
//  std::cout<< std::endl;
//  std::cout<< std::endl << "ERROR in TextureManager::getTextureWithoutRefcount():";
//  std::cout<< std::endl << "         Could not find existing texture with id = " << id << " !";
//  std::cout<< std::endl << "         You will get the TextureManagers default Texture";
//  std::cout<< std::endl << "         so your applcation won't crash.";
//  std::cout<< std::endl << "         FIX YOUR CODE NOW !!!";
//  std::flush(std::cout);
//#endif
//#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
//  std::cout<< std::endl;
//  std::cout<< std::endl << "Warning from TextureManager::getTextureWithoutRefcount():";
//  std::cout<< std::endl << "             Could not find existing texture with gid = " << gid << " !";
//  std::cout<< std::endl << "             Returning 0";
//  std::flush(std::cout);
//#endif

//  _textures[0]->takeReference();
//  return _textures[0];
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////

/// drops reference to gloost::Texture object

bool
TextureManager::dropReference(gloostId gid)
{
  auto pos = _textures.find(gid);
  if (pos != _textures.end())
  {
    if (_textures[gid]->getReferenceCount() == 1)
    {
      removeTexture(gid);
    }
    else
    {
      _textures[gid]->dropReference();
    }
    return true;
  }

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
  std::cout<< std::endl;
  std::cout<< std::endl << "Warning from TextureManager::dropReference():";
  std::cout<< std::endl << "             Could not find a texture with gid = " << gid << "!";
  std::cout<< std::endl;
  std::flush(std::cout);
#endif

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/// unloads all textures which are only referenced by the TextureManager

void
TextureManager::cleanUp()
{
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout<< std::endl;
  std::cout<< std::endl << "Message from TextureManager::cleanup():";
  std::cout<< std::endl << "             Removing all Textures with refCount == 0 from gfx-card";
  std::cout<< std::endl << "             and main memory.";
  std::cout<< std::endl;
  std::flush(std::cout);
#endif
  auto texIt    = _textures.begin();
  auto texEndIt = _textures.end();


  std::vector<gloostId> idsToBeDeleted;

  for ( ; texIt != texEndIt; ++texIt)
  {
    if ( (*texIt).second->getReferenceCount() == 1 /*&& (*texIt).first != 0*/ )
    {
      idsToBeDeleted.push_back((*texIt).first);
    }
  }

  // remove textures
  for (unsigned i=0; i!=idsToBeDeleted.size(); ++i)
  {
    removeTexture(idsToBeDeleted[i]);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

/// returns number of textures within the manager

unsigned
TextureManager::getSize() const
{
  return _textures.size();
}

//////////////////////////////////////////////////////////////////////////////////////////

/// prints infos for all textures within the manager

void
TextureManager::printTextureInfo() const
{
  auto index = _textures.begin();
  auto end   = _textures.end();
  for ( ; index != end; ++index)
  {
    std::cout << std::endl << (*index).first << " :";
    std::cout << (*(*index).second);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

/* extern */
std::ostream&
operator<< (std::ostream& os, const TextureManager& tm)
{
  os << std::endl << "TextureManager" << std::endl << "{";
  os << std::endl << "    size:    " << tm.getSize();
  os << std::endl;
  tm.printTextureInfo();
  os << std::endl;
  os << std::endl << "} // TextureManager" << std::endl;
  std::flush(os);
  return os;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
