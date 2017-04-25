
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



#ifndef H_GLOOST_TEXTUREATLAS
#define H_GLOOST_TEXTUREATLAS



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/gl/Texture.h>


// cpp includes
#include <string>



namespace gloost
{


  //  Stores multiple Textures in one big Texture and provides TexCoords for the areas

class TextureAtlas
{
	public:

    // class constructor
    TextureAtlas();

    // class destructor
	  virtual ~TextureAtlas();


    // creates a texture for the atlas
	  void createAtlasTexture( unsigned width,
                             unsigned height,
                             unsigned target,
                             unsigned internalFormat,
                             unsigned pixelFormat,
                             unsigned pixelType);

    /* copys the pixel of an texture to the atlas, returns a id for the texcoords
       Pixelformat and type of the texture has to match to the atlas texture */
    unsigned createTileFromTexture(unsigned textureId,
                                       unsigned positionX,
                                       unsigned positionY);

    ///
    gloostId getAtlasTextureGid() const;

    ///
    gl::Texture* getAtlasTexture();


    ///
    unsigned getNumEntries() const;


	protected:


	  std::map<unsigned, gloost::vec4> _tilesTexcoords;

	  gloostId _uniqueIdCounter;

    gloostId _atlasTextureGid;
	  gl::Texture*     _atlasTexture;
};


} // namespace gloost


#endif // H_GLOOST_TEXTUREATLAS



