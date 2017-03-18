
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



#ifndef H_GLOOST_TILESET
#define H_GLOOST_TILESET

// gloost system includes
#include <gloost/TextureAtlas.h>
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/UniformTypes.h>

// cpp includesvec4
#include <string>
#include <vector>

namespace gloost
{
  // class predeclared
  class Texture;


  //  Stores tiles/textures of the same size in one big texture and provides texture coordinates for the positions

class TileSet : public TextureAtlas
{
	public:

    /// class constructor
    TileSet();

    /// class destructor
	  ~TileSet();

    // loads a tile texture
    void loadFromTexture( const std::string& textureFilePath,
                          unsigned int numTilesH,
                          unsigned int numTilesV);

    // loads a tile texture
    void loadFromTexture( unsigned int textureId,
                          unsigned int numTilesH,
                          unsigned int numTilesV);

    // loads the images of a folder an creates a tile texture
    void loadFromFolder(const std::string& tilesFolderPath);

    /// returns the texture coordinates of a particular frame within the texture (starting at 0)
    const gloost::vec4& getTexCoordsForFrame(int frameNum);

    /// set the number of tiles within the texture
    void setNumFrames(unsigned int numTiles);

    ///
    unsigned int getNumTilesH() const;
    ///
    unsigned int getNumTilesV() const;

    ///
    unsigned int getTileWidth() const;
    ///
    unsigned int getTileHeight() const;

    /// returns tileWidth/tileHeight ration
    float getTileAspectRatio() const;

    ///
    void setBlendFuncNormalWithAlpha();

	private:

	  unsigned int _numTilesH;
	  unsigned int _numTilesV;
	  unsigned int _numFrames;

    float        _tileAspectRatio;

    unsigned int _tileWidth;
    unsigned int _tileHeight;

    void precalcUniformTexCoords();

};


} // namespace gloost


#endif // H_GLOOST_TILESET


