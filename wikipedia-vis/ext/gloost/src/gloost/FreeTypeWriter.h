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

#ifndef H_GLOOST_FREETYPEWRITER
#define H_GLOOST_FREETYPEWRITER

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/InterleavedAttributes.h>
#include <gloost/InterleavedAttributes.h>

// cpp includes
#include <string>
#include <memory>
#include <map>

#define GLOOST_FREETYPEWRITER_ALIGN_LEFT   0
#define GLOOST_FREETYPEWRITER_ALIGN_RIGHT  1

#define GLOOST_FREETYPEWRITER_ALIGN_BOTTOM 0
#define GLOOST_FREETYPEWRITER_ALIGN_TOP    1

#define GLOOST_FREETYPEWRITER_ALIGN_CENTER 2

namespace gloost
{

namespace gl { class ShaderProgram; class UniformSet; class Vbo; }


//  Writes Text on screen using ttf fonts (uses GNU's freetype)

class FreeTypeWriter
{
  public:

  /// monocrome pixel containing a luminance and an alpha value
  class MonocromPixel
  {

    public:

    MonocromPixel() {};
    MonocromPixel(float value): _luminance(value), _alpha(value) {};
    MonocromPixel(float luminance, float alpha): _luminance(luminance), _alpha(alpha) {};

    void  setLuminance(float luminance)
    {
      _luminance = luminance;
    }
    float getLuminance() const
    {
      return _luminance;
    }
    void  setAlpha(float alpha)
    {
      _alpha = alpha;
    }
    float getAlpha() const
    {
      return _alpha;
    }

    private:

    float _luminance;
    float _alpha;
  };

  /// stores the properties of a freetype glyph
  struct glyphSpec
  {
    glyphSpec():
      _index(0),
      _leftOffset(0),
      _topOffset(0),
      _width(0),
      _height(0),
      _texcoords(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    glyphSpec(unsigned char index,
              int leftOffset, int topOffset,
              int width, int height)
    {
      _index      = index;
      _leftOffset = leftOffset;
      _topOffset  = topOffset;
      _width      = width;
      _height     = height;
    }

    unsigned char _index;
    int           _leftOffset;
    int           _topOffset;
    int           _width;
    int           _height;
    vec4          _texcoords; // minU, maxU, minV, maxV
  };

  /// a std::shared_ptr of an ShaderProgram instance
  typedef std::shared_ptr<FreeTypeWriter>       shared_ptr;
  typedef std::shared_ptr<const FreeTypeWriter> const_shared_ptr;

  // class factory
  static shared_ptr create (const std::string& fontPath,
                            float              fontSize,
                            bool               antialiased = true,
                            unsigned           alignModeH = GLOOST_FREETYPEWRITER_ALIGN_LEFT,
                            float              charSpace  = 1.0f,
                            float              lineSpace  = 1.0f);

  // class constructor
  FreeTypeWriter(const std::string& fontPath,
                 float              fontSize,
                 bool               antialiased = true,
                 unsigned           alignModeH = GLOOST_FREETYPEWRITER_ALIGN_LEFT,
                 float              charSpace  = 1.0f,
                 float              lineSpace  = 1.0f);

  // class destructor
  ~FreeTypeWriter();

  // returns a container with the left and top bearings and horizontal and vertical size of each char
  std::vector<glyphSpec>& getGlyphSpecs();
  const std::vector<glyphSpec>& getGlyphSpecs() const;

  // returns the gloostId of the generated atlas texture with the glyphes
  gloostId getFontAtlasTextureGid() const;


  // set the number of cached text line VBOs
  void setNumCachedTextLines(unsigned number);
  // clears the cached text line VBOs
  void clearCachedTextLines();



  // binds the font atlas and setups writing
  bool beginText(const Matrix& modelViewProjectionMatrix);

  // sets the tint color of the font rendering (requires to call beginText(...) first)
  void setTintColor(const gloost::vec4& color);

  // ends the text writing
  void endText();

  // writes a line of text
  void writeLine(float x, float y, const std::string& text);
  void writeLine(float xOffset, const std::string& text);
  void writeLine(const std::string& text);

  // skips a line
  void nextLine();


  // returns the length of a line for current configuration
  float getLineLength(const std::string& text) const;

  // cuts a line, so it is not longer as maxPixelLength with the current font
  std::string cutLineToLength(const std::string& text, unsigned maxPixelLength) const;

  // sets the write position
  void setWritePos(const Point3& pos);
  void setWritePos(float x, float y);

  // returns the current writer position
  const Point3& getWritePos() const;

  // sets the horizontal align mode to GLOOST_FREETYPEWRITER_ALIGN_LEFT, GLOOST_FREETYPEWRITER_ALIGN_RIGHT or GLOOST_FREETYPEWRITER_ALIGN_CENTER
  void setAlignModeH(unsigned alignModeH);


  // returnt the fontsize
  float getFontSize() const;

  // sets the space between chars
  void setCharSpace(float charSpace);
  // returns the space between chars
  float getCharSpace() const;


  // sets the space between lines
  void setLineSpace (float lineDistance);
  // returns the space between lines
  float getLineSpace () const;
  // returns the height of a line (font height + line space)
  float getLineHeight() const;


  // sets the forced fixed Glyph
  void setForcedFixedWidth(float fixedWidth);

  // returns the forced fixed Glyph
  float getForcedFixedWidth() const;

  // inserts ...
  void insert_into_atlasComposeBuffer(unsigned char charIndex,
                                      std::vector<MonocromPixel>& monocromPixelBuffer,  unsigned bufferWidth, unsigned bufferHeight,
                                      const std::vector<MonocromPixel>& bufferToInsert, unsigned insertWidth, unsigned insertHeight,
                                      unsigned insertPosH, unsigned insertPosV);

  // sets a other shader than the default shader
  void setShader(const std::shared_ptr<gl::ShaderProgram>& shader);
  const std::shared_ptr<gl::ShaderProgram>& getShader() const;

protected:

  static InterleavedAttributes::LayoutVector _defaultLayout;

private:

  // setup
  std::string _fontPath;
  float       _fontSize;
  bool        _antialiased;

  // specs
  std::vector<glyphSpec> _glyphSpecs;

  //
  gloostId _glyphAtlasTextureGid;

  // writing
  gloost::Point3 _currentWritePosition;
  float          _charSpace;
  float          _lineSpace;

  float          _forcedGlyphWidth;
  unsigned       _alignModeH;

  std::shared_ptr<gl::ShaderProgram> _shader;
  std::shared_ptr<gl::UniformSet>    _uniforms;

  // strores text line geometry as an VBO
  std::map<std::string, std::shared_ptr<gl::Vbo> > _vboCache;
  unsigned _maxCachedVbos;

  // write routine
  void write (const std::string& text);

  void renderLine(const gloost::Point3& pos,
                  const std::shared_ptr<gloost::InterleavedAttributes>& ia) const;

  void renderLine(const gloost::Point3& pos,
                           const std::shared_ptr<gl::Vbo>& vbo) const;

};

} // namespace gloost

#endif // H_GLOOST_FREETYPEWRITER



