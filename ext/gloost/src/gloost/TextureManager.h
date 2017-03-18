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


#ifndef H_GLOOST_TEXTUREMANAGER
#define H_GLOOST_TEXTUREMANAGER

// gloost includes
#include <gloost/gloostConfig.h>
#include <gloost/gl/Texture.h>

// cpp includes
#include <map>
#include <boost/thread/mutex.hpp>


namespace gloost
{
// gl::Texture* factory and container

class TextureManager
{

public:

  ~TextureManager();

  // get an instance to the TextureManager
  static TextureManager* getInstance();
  static TextureManager* get();

  // copy contructor to avoid assignment to a non reference
  explicit TextureManager(const TextureManager&) {}


  // This methodes will create new textures and increment the reference counter of this texture//
  // Creates empty Texture object of given width and height and returns a unique gloostId (refCount +)
  // gloostId createTexture(unsigned width, unsigned height);

  // load a texture (automatic configuration of the GL_Texture_2Ds format and type) (tested: jpg, png24, png32) (refCount +)
  gloostId createTexture(std::string file_name, bool share = true);

  // load a texture (automatic configuration of the GL_Texture_2Ds format and type) (tested: jpg, png24, png32) (refCount +)
  gloostId createTexture(gloost::PathType file_name, bool share = true);

  // Add existing Texture to be managed by the TextureManager (refCount +)
  gloostId addTexture(gl::Texture* texture);

  // removes a texture from the TextureManager
  void removeTexture(gloostId id);

  // returns true if a id references a Texture
//  bool checkId (unsigned id) const;

  // returns reference to an existing Texture object and increments the reference counter (refCount +), , 0 if id was not found
  gl::Texture* getTextureReference(gloostId gid);

  // returns reference to gloost::Texture object WITHOUT incrementing the reference counter, 0 if id was not found
  gl::Texture* getTextureWithoutRefcount(gloostId gid);

  // drops reference to gloost::Texture object (refCount -)
  bool dropReference(gloostId gid);


  // unloads all unused textures
  void cleanUp();

  // returns number of textures within the manager
  unsigned getSize() const;

  // prints infos for all textures within the manager
  void printTextureInfo() const;

private:
  ///
  static TextureManager* _theinstance;

  /// map to hold the textures
  std::map<gloostId, gl::Texture*> _textures;

  /// map to hold filepath->id assoziations to share textures
  std::map<gloost::PathType, gloostId> _shareMap;

  // protects the _idCounter
  boost::mutex _idMutex;
  boost::mutex _addRemoveMutex;

  /// class constructor
  TextureManager();

  /// returns a new texture id (thread save)
  gloostId getNewTextureGid();

  /**
    Creates empty Texture with specific id object of given width and height
    This will overwrite existing textures with this id, so don't mess around with this.
  **/
  gloostId createTexture(gloostId gid, unsigned width, unsigned height);
};


/// ostream operator
extern std::ostream& operator<< (std::ostream&, const TextureManager& );
}  // namespace gloost


#endif // H_GLOOST_TEXTUREMANAGER
