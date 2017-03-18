
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
#include <gloost/TileSet.h>
#include <gloost/gloostMath.h>
#include <gloost/gl/Texture.h>
#include <gloost/TextureManager.h>
#include <gloost/util/DirectoryList.h>
#include <gloost/gloostHelper.h>



// cpp includes
#include <string>
#include <iostream>
#include <string>



namespace gloost
{


/**
  \class TileSet

  \brief Stores tiles/textures of the same size in one big texture and provides texture coordinates for the positions

  \author Felix Weiszig
  \date   Feb 2010



  \remarks 1. Class is able to load a whole tile texture or a folder with single
           tiles/frames of the same size and format.<br/>
           2. If you create a TileSet from a folder with multiple tiles, the TileSet
           class will automaticly choose a optimal, nearly square size.<br/>
           3. The order of the tiles within the AtlasTexture is as follows:

           \code
           -------------
           |00|01|02|03|
           -------------
           |04|05|06|07|
           -------------
           |...
           \endcode

*/


////////////////////////////////////////////////////////////////////////////////


  /// class constructor

TileSet::TileSet():
    TextureAtlas(),
    _numTilesH(16),
    _numTilesV(16),
    _numFrames(256),
    _tileAspectRatio(1.0f),
    _tileWidth(32),
    _tileHeight(32)

{
  precalcUniformTexCoords();
}


////////////////////////////////////////////////////////////////////////////////


  /// class destructor

TileSet::~TileSet()
{
}


////////////////////////////////////////////////////////////////////////////////



  ///

void
TileSet::loadFromTexture( const std::string& textureFilePath,
                          unsigned int numTilesH,
                          unsigned int numTilesV)
{


  _atlasTextureGid = gloost::TextureManager::getInstance()->createTexture(textureFilePath);

  _numTilesH = numTilesH;
  _numTilesV = numTilesV;

  _atlasTexture = gloost::TextureManager::getInstance()->getTextureReference(_atlasTextureGid);

  _tileWidth  = _atlasTexture->getWidth()/_numTilesH;
  _tileHeight = _atlasTexture->getHeight()/_numTilesV;

  _tileAspectRatio = _tileWidth/(float)_tileHeight;

	precalcUniformTexCoords();
}


////////////////////////////////////////////////////////////////////////////////



  ///

void
TileSet::loadFromTexture( unsigned int textureId,
                          unsigned int numTilesH,
                          unsigned int numTilesV)
{
  _atlasTextureGid = textureId;
  _numTilesH      = numTilesH;
  _numTilesV      = numTilesV;

  _atlasTexture = gloost::TextureManager::getInstance()->getTextureReference(_atlasTextureGid);

  _tileWidth  = _atlasTexture->getWidth()/_numTilesH;
  _tileHeight = _atlasTexture->getHeight()/_numTilesV;

  _tileAspectRatio = _tileWidth/(float)_tileHeight;

	precalcUniformTexCoords();
}

////////////////////////////////////////////////////////////////////////////////


  /// class constructor takes a path to a folder with frames of the same size

void
TileSet::loadFromFolder(const std::string& tilesFolderPath)
{

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout << std::endl;
//  std::cout << std::endl << "Message from TileSet::TileSet(const std::string& tilesFolderPath) on SharedResource " << getSharedResourceId() << ":";
  std::cout << std::endl << "             Creating tile texture for folder \"" << tilesFolderPath << "\"";
  std::flush(std::cout);
#endif


  gloost::TextureManager* texManager = gloost::TextureManager::getInstance();


  util::DirectoryList dir(tilesFolderPath);
  dir.open();


  const auto& files = dir.getEntries();
  std::vector<int> textureIds;


  for (unsigned int i=0; i != files.size(); ++i)
  {
    if (files[i].filename().native().length() > 4)
    {
      textureIds.push_back(texManager->createTexture(files[i].native()));
    }
  }


  /// test
  _numFrames = files.size()-2;

  gl::Texture* sample = texManager->getTextureWithoutRefcount(textureIds[0]);

  _tileWidth  = sample->getWidth();
  _tileHeight = sample->getHeight();

  _tileAspectRatio = _tileWidth/(float)_tileHeight;



  /// determine ideal texture size
  _numTilesH = 1;
  _numTilesV = 1;
  unsigned int numTilesInTexture = 1;


  while(numTilesInTexture < _numFrames)
  {
    if (_numTilesH*_tileWidth < _numTilesV*_tileHeight)
    {
      ++_numTilesH;
    }
    else
    {
      ++_numTilesV;
    }

    numTilesInTexture = _numTilesH*_numTilesV;
  }

  int atlasTextureWidth  = _tileWidth*_numTilesH;
  int atlasTextureHeight = _tileHeight*_numTilesV;



#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout << std::endl;
//  std::cout << std::endl << "Message from TileSet::TileSet(const std::string& tilesFolderPath) on SharedResource " << getSharedResourceId() << ":";
  std::cout << std::endl << "             Using following parameters:";
  std::cout << std::endl << "             Folder: ...... \"" << tilesFolderPath << "\"";
  std::cout << std::endl << "             Number of Frames .... " << _numFrames;
  std::cout << std::endl << "             Tile width: ......... " << _tileWidth;
  std::cout << std::endl << "             Tile height: ........ " << _tileHeight;
  std::cout << std::endl << "             Texture width: ...... " << atlasTextureWidth;
  std::cout << std::endl << "             Texture height: ..... " << atlasTextureHeight;
  std::cout << std::endl << "             Tiles horizontal: ... " << _numTilesH;
  std::cout << std::endl << "             Tiles vertical: ..... " << _numTilesV;
  std::flush(std::cout);
#endif


  createAtlasTexture(atlasTextureWidth,
                     atlasTextureHeight,
                     GL_TEXTURE_2D,
                     sample->getInternalFormat(),
                     sample->getPixelFormat(),
                     sample->getPixelType() );



  _atlasTexture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  _atlasTexture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);



  _atlasTexture->bind();


  int x = 0;
  int y = 0;

  /// copy all tiles into one tile texture
  for (unsigned int i=0; i!=_numFrames; ++i)
  {
     createTileFromTexture(textureIds[i],
                           x*_tileWidth,
                           y*_tileHeight);



    ++x;

    if (x == (int)(_numTilesH))
    {
      x = 0;
      ++y;
    }

    /// drop refrence of this single tile because it is no longer needed
    texManager->dropReference(textureIds[i]);

  }

  _atlasTexture->unbind();




  precalcUniformTexCoords();
}


////////////////////////////////////////////////////////////////////////////////


  ///

void
TileSet::precalcUniformTexCoords()
{
  _tilesTexcoords.clear();

  // precalculate texcoord pairs for all tiles
  unsigned int index = 0;

  for (unsigned int y=0; y!=_numTilesV; ++y)
  {
    double yMin = (1.0/_numTilesV)*y;
    double yMax = (1.0/_numTilesV)*(y+1.0);

    for (unsigned int x=0; x!=_numTilesH; ++x)
    {
      double xMin = (1.0/_numTilesH)*x;
      double xMax = (1.0/_numTilesH)*(x+1.0);
      _tilesTexcoords[index++] = gloost::vec4(xMin, xMax, yMin, yMax);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////


  /// returns the texture coordinates of a particular frame within the texture (starting at 0)

const gloost::vec4&
TileSet::getTexCoordsForFrame(int frameNum)
{
  // loop animation
  frameNum = (int)math::fmod((mathType)frameNum, (mathType)_numFrames);
  return _tilesTexcoords[(unsigned int)frameNum];
}


////////////////////////////////////////////////////////////////////////////////


 ///

void
TileSet::setNumFrames(unsigned int numFrames)
{
  _numFrames = numFrames;
}


////////////////////////////////////////////////////////////////////////////////


 ///

unsigned int
TileSet::getNumTilesH() const
{
  return _numTilesH;
}


////////////////////////////////////////////////////////////////////////////////


  ///

unsigned int
TileSet::getNumTilesV() const
{
  return _numTilesV;
}


////////////////////////////////////////////////////////////////////////////////


  ///

unsigned int
TileSet::getTileWidth() const
{
  return _tileWidth;
}


////////////////////////////////////////////////////////////////////////////////


  ///

unsigned int
TileSet::getTileHeight() const
{
  return _tileHeight;
}


////////////////////////////////////////////////////////////////////////////////


  /// returns tileWidth/tileHeight ration

float
TileSet::getTileAspectRatio() const
{
  return _tileAspectRatio;;
}


////////////////////////////////////////////////////////////////////////////////


  ///

void
TileSet::setBlendFuncNormalWithAlpha()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


////////////////////////////////////////////////////////////////////////////////


///









////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


