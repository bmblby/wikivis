
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



#ifndef H_GLOOST_IMAGE
#define H_GLOOST_IMAGE



// gloost system includes
#include <gloost/gloostMath.h>
#include <gloost/gloostConfig.h>


// cpp includes
#include <string>
#include <memory>

//#define GLOOST_GNULINUX

/// contrib includes
#ifdef GLOOST_GNULINUX
  #include <FreeImage.h>
#endif

#ifdef GLOOST_WINDOWS
  #include <windows.h>
  #include <FreeImage/FreeImage.h>
#endif


namespace gloost
{


  class BinaryBundle;

  //  Wrapper for a FIBITMAP of the libFreeImage

class Image
{
	public:

    /// a std::shared_ptr of an RendererBase instance
    typedef std::shared_ptr<Image>       shared_ptr;
    typedef std::shared_ptr<const Image> const_shared_ptr;



    static shared_ptr create(const std::string& fileName);

    static shared_ptr create(const gloost::PathType& fileName);

    static shared_ptr create(unsigned width,
                             unsigned height,
                             unsigned bitsPerPixel,
                             unsigned char* pixels);

    // class destructor
	  virtual ~Image();

    // returns the filename of this image
    const std::string& getFileName() const;


	  // returns the width of the image in pixels
	  unsigned getWidth() const;

	  // returns the height of the image in pixels
	  unsigned getHeight() const;

    // returns ratio width/height
	  mathType getAspectRatio() const;



	  // returns the number of bit per pixel
	  unsigned getBitsPerPixel() const;

	  // returns the number of color components per pixel
	  unsigned getComponentsPerPixel() const;

	  // returns true if the Image has pixels
	  bool hasPixels() const;


	  // returns the pixel data of the image
	  unsigned char* getPixelData();

	  // returns the pixel data of the image
	  const unsigned char* getPixelData() const;



    // fills the Image (color components from 0.0 .. 1.0)
    void fill(mathType r, mathType g, mathType b, mathType a);

    // flips an Image horizontally
    void flipH();

    // flips an Image vertically
    void flipV();

    // rotates the image around its center
//    void rotate(mathType angleInDegrees);

    // scales an Image
//    void scale (unsigned width, unsigned height, FREE_IMAGE_FILTER filter);


    // Sets the color of pixel x,y
    void setPixel(unsigned x,
                  unsigned y,
                  mathType r,
                  mathType g,
                  mathType b,
                  mathType a);

    // returns the color of pixel x,y
    vec4 getPixelColor(unsigned x, unsigned y) const;

    // samples an image bilinear
    vec4 sampleBilinear(float x, float y) const;

    void writeToPNG(const std::string& path);
    void writeToPNG(const gloost::PathType& path);

    void writeAsThumbnail(unsigned maxPixelSize, const std::string& path);
    void writeAsThumbnail(unsigned maxPixelSize, const gloost::PathType& path);

    shared_ptr getAsThumbnail(unsigned maxPixelSize);

  protected:

    Image(const gloost::PathType& fileName);

    Image(unsigned width,
          unsigned height,
          unsigned bitsPerPixel,
          unsigned char* pixels = nullptr);

    Image(FIBITMAP*);

	private:

    // DO YOU HAVE AN COMPILER ERROR HERE? <<-----
    // For GNU/LINUX: add a define called GLOOST_GNULINUX to your makefile
    // For WINDOWS: I DONT KNOW
    FIBITMAP* _freeImage_bitmap;
};


} // namespace gloost


#endif // H_GLOOST_IMAGE


