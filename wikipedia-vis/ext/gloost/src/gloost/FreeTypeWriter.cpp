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
#include <gloost/FreeTypeWriter.h>
#include <gloost/TextureManager.h>
#include <gloost/TileSet.h>
#include <gloost/gl/ShaderProgram.h>
#include <gloost/gl/UniformSet.h>
#include <gloost/gl/Vbo.h>
#include <gloost/gl/GlGarbageCollector.h>
#include <gloost/gloostConfig.h>
#include <gloost/gl/gloostGlUtil.h>

//FreeType Headers
#ifdef GLOOST_SYSTEM_FREETYPE2   // <-- This is set in gloostConfig.h
  #include <freetype2/ft2build.h>
  #include <freetype2/freetype.h>
  #include <freetype2/ftglyph.h>
  #include <freetype2/ftoutln.h>
  #include <freetype2/fttrigon.h>
#else
  #include <ft2build.h>
  #include <freetype/freetype.h>
  #include <freetype/ftglyph.h>
  #include <freetype/ftoutln.h>
  #include <freetype/fttrigon.h>
#endif

// cpp includes
#include <string>
#include <iostream>


namespace gloost
{

/**
  \class   FreeTypeWriter
  \brief   Writes Text on screen using ttf fonts (uses GNU's freetype)
  \author  Felix Weiszig
  \date    May 2011
  \remarks Much of the code was taken from Nehe lesson 43, written by Jeff Molofee
           This is his original message:
           <i>
             This code was created by Jeff Molofee '99
             (ported to Linux/SDL by Ti Leggett '01)
             If you've found this code useful, please let me know.
             Visit Jeff at http://nehe.gamedev.net/
             or for port-specific comments, questions, bugreports etc.
             email to leggett@eecs.tulane.edu
           </i>
*/

////////////////////////////////////////////////////////////////////////////////

/*FreeTypeWriter*/ InterleavedAttributes::LayoutVector FreeTypeWriter::_defaultLayout = {{3,12,"in_position"}, {2,8,"in_texCoords"} };

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   class factory
  \remarks ...
*/

/*static*/
FreeTypeWriter::shared_ptr
FreeTypeWriter::create(const std::string& fontPath,
                       float              fontSize,
                       bool               antialiased,
                       unsigned           alignModeH,
                       float              charSpace,
                       float              lineSpace)
{
  auto instance = shared_ptr(new FreeTypeWriter(fontPath, fontSize, antialiased,
                             alignModeH, charSpace, lineSpace));
  return instance;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

FreeTypeWriter::FreeTypeWriter(const    std::string& fontPath,
                               float    fontSize,
                               bool     antialiased,
                               unsigned alignModeH,
                               float    charSpace,
                               float    lineSpace):
  _fontPath(fontPath),
  _fontSize(fontSize),
  _antialiased(antialiased),
  _glyphSpecs(),
  _glyphAtlasTextureGid(0),
  _currentWritePosition(),
  _charSpace(charSpace),
  _lineSpace(lineSpace),
  _forcedGlyphWidth(0),
  _alignModeH(alignModeH),
  _shader(nullptr),
  _uniforms(new gl::UniformSet()),
  _vboCache(),
  _maxCachedVbos(20u)
{
  FT_Library library;

  // Create and initilize a freetype font library.
  if (FT_Init_FreeType( &library ))
  {
    throw std::runtime_error("FT_Init_FreeType failed");
  }

  //The object in which Freetype holds information on a given
  //font is called a "face".
  FT_Face face;

  //This is where we load in the font information from the file.
  //Of all the places where the code might die, this is the most likely,
  //as FT_New_Face will die if the font file does not exist or is somehow broken.
  if (FT_New_Face( library, fontPath.c_str(), 0, &face ))
  {
//    throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

    std::cerr << std::endl;
    std::cerr << std::endl << "D'OH from FreeTypeWriter::FreeTypeWriter(): ";
    std::cerr << std::endl << "          FT_New_Face failed.";
    std::cerr << std::endl << "          There is probably a problem with your font file path.";
    std::cerr << std::endl << "          currently set to: \"" << fontPath << "\"";

    return;
  }

  //For some twisted reason, Freetype measures font size
  //in terms of 1/64ths of pixels.  Thus, to make a font
  //h pixels high, we need to request a size of h*64.
  FT_Set_Char_Size( face, fontSize*64, fontSize*64, 72, 72);

  const unsigned numCharsToSupport = 128u;
  std::vector<unsigned> texIds(numCharsToSupport, 0);
  _glyphSpecs = std::vector<glyphSpec>(numCharsToSupport, glyphSpec());
  int maxGlyphWidth  = 0;
  int maxGlyphHeight = 0;

  std::vector<std::vector<MonocromPixel> > pixelVectors(numCharsToSupport, std::vector<MonocromPixel>());

  //This is where we actually create each of the fonts display lists.
  for(unsigned char charIndex=0; charIndex<numCharsToSupport; charIndex++)
  {
    // The first thing we do is get FreeType to render our character
    // into a bitmap.  This actually requires a couple of FreeType commands:

    // Load the Glyph for our character.
    if(FT_Load_Glyph( face, FT_Get_Char_Index( face, charIndex ), FT_LOAD_DEFAULT ))
    {
      throw std::runtime_error("FT_Load_Glyph failed");
    }

    // Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
    {
      throw std::runtime_error("FT_Get_Glyph failed");
    }


    // Convert the glyph to a bitmap.
    if (antialiased)
    {
      FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
    }
    else
    {
      FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_MONO, 0, 1 );
    }

    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

    //This reference will make accessing the bitmap easier
    FT_Bitmap& bitmap=bitmap_glyph->bitmap;

    // Use our helper function to get the widths of
    // the bitmap data that we will need in order to create
    // our texture.
    int width  = bitmap.width;
    int height = bitmap.rows;
    maxGlyphWidth   = gloost::math::max(maxGlyphWidth, width);
    maxGlyphHeight  = gloost::math::max(maxGlyphHeight, height);

    // store the glyphs attributes to use them later while rendering
    _glyphSpecs[charIndex]._width      = width;
    _glyphSpecs[charIndex]._height     = height;
    _glyphSpecs[charIndex]._leftOffset = bitmap_glyph->left;
    _glyphSpecs[charIndex]._topOffset  = bitmap_glyph->top;


    // Allocate memory for the texture data.
    unsigned bufferSize =  width * height;
    GLfloat* expanded_data = new GLfloat[bufferSize*2u];

    std::vector<MonocromPixel> monocromPixels(bufferSize,
        MonocromPixel(0.0f, 0.0f));

    // Here we fill in the data for the expanded bitmap.
    // Notice that we are using two channel bitmap (one for
    // luminocity and one for alpha), but we assign
    // both luminocity and alpha to the value that we
    // find in the FreeType bitmap.
    // We use the ?: operator so that value which we use
    // will be 0 if we are in the padding zone, and whatever
    // is the the Freetype bitmap otherwise.

    if (antialiased)
    {
      for(int y=0; y != height; ++y)
      {
        for(int x=0; x != width; ++x)
        {
          {
            int readIndex  = (x + y*width);
            // invert y
            int writeIndex = (x + (height-y-1) * width) * 2;
            expanded_data[writeIndex] = 1.0f;
            expanded_data[writeIndex+1] = bitmap.buffer[readIndex]/255.0f;  // <- alpha

            monocromPixels[writeIndex*0.5].setLuminance(1.0f);
            monocromPixels[writeIndex*0.5].setAlpha(bitmap.buffer[readIndex]/255.0f);

          }
        }
      }
    }
    else
    {
      //-----------------------------------
      // true type packs monochrome info in a
      // 1-bit format, hella funky
      // here we unpack it:
      unsigned char *src = bitmap.buffer;
      for(int y=0; y != (int)bitmap.rows; ++y)
      {

        int yi = (int)bitmap.rows-y-1;

        unsigned char b = 0;
        unsigned char *bptr = src;
        for(int x=0; x != (int)bitmap.width ; ++x)
        {
          int index = 2*((x)+(yi)*width);
          expanded_data[index] = 1.0f;               // <- luminance
          monocromPixels[index*0.5].setLuminance(1.0f);

          if (x%8==0)
          {
            b = (*bptr++);
          }
          expanded_data[index + 1] = b&0x80 ? 1 : 0; // <- alpha
          monocromPixels[index*0.5].setAlpha(b&0x80 ? 1 : 0);
          b <<= 1;
        }
        src += bitmap.pitch;
      }
      //-----------------------------------
    }
    delete [] expanded_data;
    delete glyph;
    pixelVectors[charIndex] = monocromPixels;
  }

  //We don't need the face information now that the display
  FT_Done_Face(face);


  // fix bug: fill up _glyphSpecs[...]._width
  for (unsigned int charIndex=0; charIndex!=_glyphSpecs.size(); ++charIndex)
  {
    if (_glyphSpecs[charIndex]._width == 0)
    {
      _glyphSpecs[charIndex]._width = maxGlyphWidth*0.5f;
    }
  }

  // create the atlas texture
  int numTilesH = 16;
  int numTilesV = 8;

  // create the atlas Buffer
  const size_t fontAtlasWidth      = maxGlyphWidth*numTilesH;
  const size_t fontAtlasHeight     = maxGlyphHeight*numTilesV;
  const size_t num_pixels_in_atlas = fontAtlasWidth*fontAtlasHeight;

  std::vector<MonocromPixel> atlas_composition_buffer = std::vector<MonocromPixel>(num_pixels_in_atlas, MonocromPixel(0.0f, 0.0f));

  // copy single glyph pixel data into atlas pixel data
  {
    int x = 0;
    int y = 0;
    for(unsigned char charIndex=0u; charIndex!=numCharsToSupport; charIndex++)
    {
      {
        insert_into_atlasComposeBuffer(charIndex,
                                       atlas_composition_buffer, fontAtlasWidth, fontAtlasHeight,
                                       pixelVectors[charIndex], _glyphSpecs[charIndex]._width, _glyphSpecs[charIndex]._height,
                                       x*maxGlyphWidth, ((int)numTilesV-y)*maxGlyphHeight);
      }
      ++x;
      if (x == (int)(numTilesH))
      {
        x = 0;
        ++y;
      }
    }
  }

  gl::Texture* newtlasTexture = new gl::Texture(fontAtlasWidth,
                                                fontAtlasHeight,
                                                1,
                                                (unsigned char*)(&atlas_composition_buffer.front()),
                                                8,
                                                GL_TEXTURE_2D,
                                                GL_RG,
                                                GL_RG,
                                                GL_FLOAT);

  newtlasTexture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  newtlasTexture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  _glyphAtlasTextureGid = TextureManager::get()->addTexture(newtlasTexture);

  // Ditto for the library.
  FT_Done_FreeType(library);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

void
FreeTypeWriter::insert_into_atlasComposeBuffer(unsigned char charIndex,
    std::vector<MonocromPixel>& monocromPixelBuffer,  unsigned bufferWidth,   unsigned bufferHeight,
    const std::vector<MonocromPixel>& spritePixelBuffer, unsigned spriteWidth, unsigned spriteHeight,
    unsigned insertPosH, unsigned insertPosV)
{
  for (unsigned posY=0; posY!=spriteHeight/* && posY+insertPosV < bufferHeight*/; ++posY)
  {
    for (unsigned posX=0; posX!=spriteWidth/* && posX+insertPosH < bufferWidth*/; ++posX)
    {
      const size_t serialReadIndex  = gloost::math::coord2dToSerialIndex(posX, posY, spriteWidth);
      const size_t serialWriteIndex = gloost::math::coord2dToSerialIndex(posX+insertPosH, posY+insertPosV, bufferWidth);

      if (serialReadIndex >= spritePixelBuffer.size() || serialWriteIndex >= monocromPixelBuffer.size() )
      {
//        std::cerr << std::endl;
//        std::cerr << std::endl << "D'OH from FreeTypeWriter::insert_into_atlasComposeBuffer(): ";
//        std::cerr << std::endl << "          read or write index out of bound";
//        std::cerr << std::endl << "read index: "  << serialReadIndex;
//        std::cerr << std::endl << "buffer size: " << spritePixelBuffer.size();
//        std::cerr << std::endl << "read pos:   (" << posX << ", " << posY << ")";
//        std::cerr << std::endl;
//        std::cerr << std::endl << "write index: " << serialWriteIndex;
//        std::cerr << std::endl << "buffer size: " << monocromPixelBuffer.size();
//        std::cerr << std::endl << "dimension:   " << monocromPixelBuffer.size();
//        std::cerr << std::endl << "write pos:   (" << posX+insertPosH << ", " << posY+insertPosV << ")";
//        std::cerr << std::endl;
        return;
      }

      // insert to atlas buffer
      monocromPixelBuffer[serialWriteIndex] = spritePixelBuffer[serialReadIndex];
    }
  }

  float normX = 1.0/(float)bufferWidth;
  float normY = 1.0/(float)bufferHeight;

  _glyphSpecs[charIndex]._texcoords = vec4(insertPosH*normX, (insertPosH+spriteWidth)*normX,
                                      insertPosV*normY, (insertPosV+spriteHeight)*normY);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

FreeTypeWriter::~FreeTypeWriter()
{
//  gloost::TextureManager::get()->dropReference(_glyphAtlasTextureGid);
  clearCachedTextLines();
  gl::GlGarbageCollector::putTextureGid(_glyphAtlasTextureGid);
  gl::GlGarbageCollector::put(_shader);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a container with the left and top bearings and horizontal and vertical size of each char
  \param   ...
  \remarks ...
*/

std::vector<FreeTypeWriter::glyphSpec>&
FreeTypeWriter::getGlyphSpecs()
{
  return _glyphSpecs;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a container with the left and top bearings and horizontal and vertical size of each char
  \param   ...
  \remarks ...
*/

const std::vector<FreeTypeWriter::glyphSpec>&
FreeTypeWriter::getGlyphSpecs() const
{
  return _glyphSpecs;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the gloostId of the generated atlas texture with the glyphes
  \param   ...
  \remarks ...
*/

gloostId
FreeTypeWriter::getFontAtlasTextureGid() const
{
  return _glyphAtlasTextureGid;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   set the number of cached text line VBOs
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::setNumCachedTextLines(unsigned number)
{
  _maxCachedVbos = number;
  clearCachedTextLines();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   clears the cached text line VBOs
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::clearCachedTextLines()
{
  while (_vboCache.size())
  {
    auto it = _vboCache.begin();
    auto vbo = it->second;
    gloost::gl::GlGarbageCollector::put(it->second);
    it = _vboCache.erase(it);
  }
  _vboCache.clear();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   binds the font atlas and setups writing
  \param   ...
  \remarks ...
*/

bool
FreeTypeWriter::beginText(const Matrix& modelViewProjectionMatrix)
{
  static std::string defaultVertexShaderSource = "\
    #version 330 \n \
    layout(location = 0) in vec3 in_position;\
    layout(location = 1) in vec2 in_texcoords;\
    uniform mat4 u_mvpMatrix;\
    uniform vec3 u_onScreenOffset;\
    out vec2 vs_texcoords;\
    void main(void)\
    {\
      gl_Position  = u_mvpMatrix * vec4(u_onScreenOffset+in_position, 1.0);\
      vs_texcoords = in_texcoords;\
    }";

  static std::string defaultFragmentShaderSource = "\
    #version 330 \n \
    uniform sampler2D u_texture;\
    uniform vec4      u_tintColor;\
    in vec2 vs_texcoords;\
    layout(location = 0) out vec4 out_color;\
    void main(void)\
    {\
      out_color = texture(u_texture, vs_texcoords).g*u_tintColor;\
    }";


  if (_glyphAtlasTextureGid)
  {

    if (!_shader)
    {
      _shader = gloost::gl::ShaderProgram::create();
      _shader->attachShaderSource(GLOOST_SHADERPROGRAM_VERTEX_SHADER,   defaultVertexShaderSource);
      _shader->attachShaderSource(GLOOST_SHADERPROGRAM_FRAGMENT_SHADER, defaultFragmentShaderSource);
    }

    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                        GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
    _shader->use();
    _uniforms->set_sampler("u_texture", _glyphAtlasTextureGid);
    _uniforms->applyToShader(_shader);
    _shader->setUniform("u_mvpMatrix", modelViewProjectionMatrix);
    _shader->setUniform("u_tintColor", vec4(1.0f,1.0f,1.0f,1.0f));
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the tint color of the font rendering)
  \remarks requires to call beginText(...) first
*/

void
FreeTypeWriter::setTintColor(const gloost::vec4& color)
{
  if (_shader) _shader->setUniform("u_tintColor", color);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ends the text writing
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::endText()
{
  if (_shader) _shader->disable();
//  auto fontTexture = gloost::TextureManager::get()->getTextureWithoutRefcount(_glyphAtlasTextureGid);
//  if (_glyphAtlasTextureGid && fontTexture)
//  {
//    fontTexture->bind();
//  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a line of text
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::writeLine(float x, float y, const std::string& text)
{
  _currentWritePosition = gloost::Point3(x,y,0.0f);
  write(text);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a line of text
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::writeLine(float xOffset, const std::string& text)
{
  nextLine();
  _currentWritePosition[0] += xOffset;
  write(text);
  _currentWritePosition[0] -= xOffset;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a line of text
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::writeLine(const std::string& text)
{
  nextLine();
  write(text);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   skips one line
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::nextLine()
{
  _currentWritePosition[1] -= _fontSize + _lineSpace;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief       returns the length of a line for current configuration
  \param   ...
  \remarks ...
*/

float
FreeTypeWriter::getLineLength(const std::string& text) const
{
  if (_glyphSpecs.size() < 31 || !text.size())
  {
    return 0.f;
  }

  float posX = 0;
  const char* chars = text.c_str();

  for (unsigned int c=0; c!=text.length(); ++c)
  {
    auto index = (unsigned)chars[c];
    if (index > _glyphSpecs.size())
    {
      index = 32;
    }
    const glyphSpec& specs = getGlyphSpecs()[index];
    if (_forcedGlyphWidth)
    {
      posX += _forcedGlyphWidth + _charSpace;
    }
    else
    {
      posX += (specs._width + _charSpace);
    }
  }
  return posX;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   cuts a line, so it is not longer as maxPixelLength with the current font
  \param   ...
  \remarks ...
*/

std::string
FreeTypeWriter::cutLineToLength(const std::string& text, unsigned maxPixelLength) const
{
  float currentLength = 0.0f;
  std::string returnString = "";
  const char* chars = text.c_str();

  unsigned index = 0u;
  for (unsigned int c=0; c!=text.length(); ++c)
  {
    index = chars[c];
    if (index > _glyphSpecs.size())
    {
      index = 32u;
    }

    const glyphSpec& specs = getGlyphSpecs()[index];
    float nextCharLength = (specs._width + specs._leftOffset + _charSpace);
    if (currentLength + nextCharLength > maxPixelLength)
    {
      return text.substr( 0, c);
    }
    currentLength += nextCharLength;
  }
  return std::string(text);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   // writes a line of text
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::write(const std::string& text)
{
  if (_glyphSpecs.size() < 33u)
  {
    return;
  }

  Point3 alignmentOffset(0.0,0.0,0.0);
  if (_alignModeH != GLOOST_FREETYPEWRITER_ALIGN_LEFT)
  {
    float lineLength = getLineLength(text);

    if (_alignModeH == GLOOST_FREETYPEWRITER_ALIGN_CENTER)
    {
      alignmentOffset[0] -= lineLength*0.5;
    }
    else if (_alignModeH == GLOOST_FREETYPEWRITER_ALIGN_RIGHT)
    {
      alignmentOffset[0] -= lineLength;
    }
  }

  auto textLineGeometry = InterleavedAttributes::create(_defaultLayout);
  auto& attribContainer = textLineGeometry->getVector();
  auto it = _vboCache.find(text);
  if ( it != _vboCache.end() )
  {
    if (it->second->getVertexAttributes()->getNumPackages())
    {
      renderLine(alignmentOffset + _currentWritePosition, it->second);
    }
    return;
  }

  // reserve 6 vertices per char (6 vertices * 5 values)
  attribContainer.reserve(text.length()*30u);

  const char* chars = text.c_str();
  unsigned index = 0u;
  gloost::Point3 geoPosition(0.0,0.0,0.0);
  for (unsigned int c=0u; c!=text.length(); ++c)
  {
    index = chars[c];
    if (index > _glyphSpecs.size())
    {
      index = 32u;
    }

    const auto& glyphSpecs = _glyphSpecs[index];
//    auto geoPosition = alignmentOffset;
    const auto offsetY = -(glyphSpecs._height-glyphSpecs._topOffset);

//       -------
//       | \   |
//       |  \  |
//       |   \ |
//       |    \|
//       -------
    // 0,0
    attribContainer.push_back(geoPosition[0]);
    attribContainer.push_back(geoPosition[1]+offsetY);
    attribContainer.push_back(geoPosition[2]);
    attribContainer.push_back(glyphSpecs._texcoords.r);
    attribContainer.push_back(glyphSpecs._texcoords.b);

    // 0,1
    attribContainer.push_back(geoPosition[0]+glyphSpecs._width);
    attribContainer.push_back(geoPosition[1]+offsetY);
    attribContainer.push_back(geoPosition[2]);
    attribContainer.push_back(glyphSpecs._texcoords.g);
    attribContainer.push_back(glyphSpecs._texcoords.b);

    // 0,2
    attribContainer.push_back(geoPosition[0]);
    attribContainer.push_back(geoPosition[1]+glyphSpecs._height+offsetY);
    attribContainer.push_back(geoPosition[2]);
    attribContainer.push_back(glyphSpecs._texcoords.r);
    attribContainer.push_back(glyphSpecs._texcoords.a);

    // 1,0
    attribContainer.push_back(geoPosition[0]+glyphSpecs._width);
    attribContainer.push_back(geoPosition[1]+offsetY);
    attribContainer.push_back(geoPosition[2]);
    attribContainer.push_back(glyphSpecs._texcoords.g);
    attribContainer.push_back(glyphSpecs._texcoords.b);

    // 1,1
    attribContainer.push_back(geoPosition[0]+glyphSpecs._width);
    attribContainer.push_back(geoPosition[1]+glyphSpecs._height+offsetY);
    attribContainer.push_back(geoPosition[2]);
    attribContainer.push_back(glyphSpecs._texcoords.g);
    attribContainer.push_back(glyphSpecs._texcoords.a);

    // 1,2
    attribContainer.push_back(geoPosition[0]);
    attribContainer.push_back(geoPosition[1]+glyphSpecs._height+offsetY);
    attribContainer.push_back(geoPosition[2]);
    attribContainer.push_back(glyphSpecs._texcoords.r);
    attribContainer.push_back(glyphSpecs._texcoords.a);

    // offset the position for the next char
    if (_forcedGlyphWidth) geoPosition[0] += _forcedGlyphWidth;
    else                   geoPosition[0] += (glyphSpecs._width + _charSpace);

    ++index;
  }

  auto vbo = gloost::gl::Vbo::create(textLineGeometry);
//  if (_vboCache.size() < _maxCachedVbos)
//  {
//    _vboCache[text] = vbo;
//  }

  if (textLineGeometry->getNumPackages())
  {
    renderLine(alignmentOffset + _currentWritePosition, vbo);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::renderLine(const gloost::Point3& pos,
                           const std::shared_ptr<InterleavedAttributes>& ia) const
{
  if (_shader)
  {
    _shader->setUniform("u_onScreenOffset", vec3(pos[0],pos[1],pos[2]));
  }
  auto vbo = gloost::gl::Vbo::create(ia);
  vbo->bind();
  {
    vbo->draw(GL_TRIANGLES);
  }
  vbo->unbind();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::renderLine(const gloost::Point3& pos,
                           const std::shared_ptr<gl::Vbo>& vbo) const
{
  if (_shader)
  {
    _shader->setUniform("u_onScreenOffset", vec3(pos[0],pos[1],pos[2]));
  }
  if (vbo)
  {
    vbo->bind();
    {
      vbo->draw(GL_TRIANGLES);
    }
    vbo->unbind();
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the write position
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::setWritePos(const Point3& pos)
{
  _currentWritePosition = pos;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the write position
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::setWritePos(float x, float y)
{
  _currentWritePosition[0] = x;
  _currentWritePosition[1] = y;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the current writer position
  \param   ...
  \remarks ...
*/

const Point3&
FreeTypeWriter::getWritePos() const
{
  return _currentWritePosition;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the horizontal align mode to GLOOST_FREETYPEWRITER_ALIGN_LEFT,
           GLOOST_FREETYPEWRITER_ALIGN_RIGHT or GLOOST_FREETYPEWRITER_ALIGN_CENTER
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::setAlignModeH(unsigned alignModeH)
{
  _alignModeH = alignModeH;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returnt the fontsize
  \param   ...
  \remarks ...
*/

float
FreeTypeWriter::getFontSize() const
{
  return _fontSize;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the space between chars
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::setCharSpace(float charSpace)
{
  _charSpace = charSpace;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the space between chars
  \param   ...
  \remarks ...
*/

float
FreeTypeWriter::getCharSpace() const
{
  return _charSpace;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the space between lines
  \param   ...
  \remarks ...
*/

void
FreeTypeWriter::setLineSpace(float lineSpace)
{
  _lineSpace = lineSpace;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the space between lines
  \param   ...
  \remarks ...
*/

float
FreeTypeWriter::getLineSpace() const
{
  return _lineSpace;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the height of a line (font height + line space)
  \param   ...
  \remarks ...
*/

float
FreeTypeWriter::getLineHeight() const
{
  return _lineSpace+_fontSize;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the forced fixed Glyph
  \param   ...
  \remarks This is a quick hack to support monospace fonts
*/

void
FreeTypeWriter::setForcedFixedWidth(float fixedWidth)
{
  _forcedGlyphWidth = fixedWidth;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the forced fixed Glyph
  \param   ...
  \remarks This is a quick hack to support monospace fonts
*/

float
FreeTypeWriter::getForcedFixedWidth() const
{
  return _forcedGlyphWidth;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets a other shader than the default shader
  \param   ...
  \remarks if shader is nullptr, the default shader is set when calling beginText the next time
*/

void
FreeTypeWriter::setShader(const std::shared_ptr<gl::ShaderProgram>& shader)
{
  _shader = shader;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets a other shader than the default shader
  \param   ...
  \remarks The ShaderProgram instance might be null if beginText was not called befor
*/

const std::shared_ptr<gl::ShaderProgram>&
FreeTypeWriter::getShader() const
{
  return _shader;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   operator for notation MonocromPixel*mathType
  \param   ...
*/

/*extern*/
FreeTypeWriter::MonocromPixel
operator* (const FreeTypeWriter::MonocromPixel& mp, const mathType& factor)
{
  return FreeTypeWriter::MonocromPixel(mp.getLuminance() * factor,
                                       mp.getAlpha() * factor);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost

