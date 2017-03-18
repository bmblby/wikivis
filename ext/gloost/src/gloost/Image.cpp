
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
#include <gloost/Image.h>
#include <gloost/gloostConfig.h>
#include <gloost/BinaryBundle.h>
#include <gloost/BinaryFile.h>
#include <gloost/log/GlobalLog.h>


// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class Image

  \brief Wrapper for a FIBITMAP of the libFreeImage

  \author Felix Weiszig
  \date   January 2011
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/*static*/
Image::shared_ptr
Image::create(const std::string& fileName)
{
  auto instance = Image::shared_ptr(new Image(boost::filesystem::path(fileName)));
  return instance;
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
Image::shared_ptr
Image::create(const gloost::PathType& fileName)
{
  auto instance = Image::shared_ptr(new Image(fileName));
  return instance;
}


////////////////////////////////////////////////////////////////////////////////


/*static*/
Image::shared_ptr
Image::create(unsigned width,
              unsigned height,
              unsigned bitsPerPixel,
              unsigned char* pixels)
{
  auto instance = Image::shared_ptr(new Image(width, height, bitsPerPixel, pixels));
  return instance;
}


////////////////////////////////////////////////////////////////////////////////


Image::Image(const gloost::PathType& fileName):
  _freeImage_bitmap(nullptr)
{
  FreeImage_Initialise();

  if(gloost::BinaryFile::fileExists(fileName))
  {
    //Automatocally detects the format(from over 20 formats!)
    FREE_IMAGE_FORMAT imageFileType;
    #ifdef GLOOST_WINDOWS
      imageFileType = FreeImage_GetFileTypeU(fileName.wstring().c_str(),0);
    #else
      imageFileType = FreeImage_GetFileType(fileName.c_str(),0);
    #endif
    /// error message if file type is unkown
    if (imageFileType == FIF_UNKNOWN)
    {
  #ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
      std::cout << std::endl;
       std::cout << std::endl << "ERROR in Image::Image(const std::string& fileName): ";
      std::cout << std::endl << "             Could NOT load image \"" << fileName << "\".";
      std::cout << std::endl << "             File path is wrong or the file has an unsupported file type.";
      std::flush(std::cout);
  #endif
      return;
    }

    /// load a FreeImage
    #ifdef GLOOST_WINDOWS
      _freeImage_bitmap = FreeImage_LoadU(imageFileType, fileName.wstring().c_str());
    #else
      _freeImage_bitmap = FreeImage_Load(imageFileType, fileName.string().c_str());
    #endif

    FreeImage_FlipVertical(_freeImage_bitmap);



  #ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
      std::cout << std::endl;
      std::cout << std::endl << "Message from Image::Image(const std::string& fileName): ";
      std::cout << std::endl << "             Image \"" << fileName << "\" loaded";
      std::cout << std::endl << "             width: .......        " << getWidth() << " pix";
      std::cout << std::endl << "             height: ......        " << getHeight() << " pix";
      std::cout << std::endl << "             bit per pixel:        " << getBitsPerPixel();
      std::flush(std::cout);
  #endif
  }
  else
  {
    gloost::log::GlobalLog::postDebug(GLOOST_CURRENT_LOCATION,
                                      "Source file not valid!");

  }



}


////////////////////////////////////////////////////////////////////////////////


Image::Image(unsigned width,
             unsigned height,
             unsigned bitsPerPixel,
             unsigned char* pixels):
  _freeImage_bitmap(nullptr)
{
  FreeImage_Initialise();

  /// allocate memory for a new FreeImage bitmap
  _freeImage_bitmap = FreeImage_Allocate(width, height, bitsPerPixel);

  if(pixels)
  {
    RGBQUAD color;
    for (size_t y = 0; y != height; ++y)
    {
      for (size_t x = 0; x !=width; ++x)
      {
        unsigned i = (x + y * width )*4;

  #if 0
        float value_r = std::min(1.0f, buffer[i])   * 255.0f;
        float value_g = std::min(1.0f, buffer[i+1]) * 255.0f;
        float value_b = std::min(1.0f, buffer[i+2]) * 255.0f;
        float value_a = std::min(1.0f, buffer[i+3]) * 255.0f;


        color.rgbRed =  value_r;
        color.rgbGreen =  value_g;
        color.rgbBlue =  value_b;
        color.rgbReserved =  value_a;
  #endif // 0

        color.rgbRed =  pixels[i];
        color.rgbGreen =  pixels[i+1];
        color.rgbBlue =  pixels[i+2];
        color.rgbReserved =  pixels[i+3];

        FreeImage_SetPixelColor(_freeImage_bitmap,x,y, &color);
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////////


Image::Image(FIBITMAP* bitmap):
  _freeImage_bitmap(bitmap)
{
  FreeImage_Initialise();
}


////////////////////////////////////////////////////////////////////////////////



/*virtual*/
Image::~Image()
{
  if (_freeImage_bitmap)
  {
    FreeImage_Unload(_freeImage_bitmap);
  }
  FreeImage_DeInitialise();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the width of the image in pixels
  \param ...
  \remarks ...
*/

unsigned int
Image::getWidth() const
{
	return FreeImage_GetWidth(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the height of the image in pixels
  \param ...
  \remarks ...
*/

unsigned int
Image::getHeight() const
{
	return FreeImage_GetHeight(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns ratio width/height
  \param ...
  \remarks ...
*/

mathType
Image::getAspectRatio() const
{
	return getWidth()/(mathType)getHeight();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the number of bit per pixel
  \param ...
  \remarks ...
*/

unsigned int
Image::getBitsPerPixel() const
{
	return FreeImage_GetBPP(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the number of color components per pixel
  \param ...
  \remarks note this is hacked!!
*/

unsigned int
Image::getComponentsPerPixel() const
{
	return 4;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns true if the Image has pixels
  \param ...
  \remarks ...
*/

bool
Image::hasPixels() const
{
	return (bool)_freeImage_bitmap;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the pixel data of the image
  \param ...
  \remarks ...
*/

unsigned char*
Image::getPixelData()
{
	return (unsigned char*)FreeImage_GetBits(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the pixel data of the image
  \param ...
  \remarks ...
*/

const unsigned char*
Image::getPixelData() const
{
	return (unsigned char*)FreeImage_GetBits(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the pixel data of the image
  \param ...
  \remarks ...
*/

vec4
Image::getPixelColor(unsigned x, unsigned y) const
{
  RGBQUAD color;
  FreeImage_GetPixelColor(_freeImage_bitmap, x, y, &color);
  return vec4(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the pixel data of the image
  \param ...
  \remarks ...
*/

vec4
Image::sampleBilinear(float /*x*/, float /*y*/) const
{
//  RGBQUAD color;
//  FreeImage_GetPixelColor(_freeImage_bitmap, x, y, &color);
//  return vec4(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
    return vec4(0.0f,0.0f,0.0f,0.0f);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   fills the Image with a color
  \param   r red color component between 0.0 ... 1.0
  \param   g green color component between 0.0 ... 1.0
  \param   b blue color component between 0.0 ... 1.0
  \param   a alpha color component between 0.0 ... 1.0
  \remarks ...
*/

void
Image::fill(mathType r, mathType g, mathType b, mathType a)
{
  // FreeImage uses BGRA scheme
  RGBQUAD color;
  color.rgbBlue       = (unsigned int) gloost::math::clamp(b*255.0, 0.0, 255.0);
  color.rgbGreen      = (unsigned int) gloost::math::clamp(g*255.0, 0.0, 255.0);
  color.rgbRed        = (unsigned int) gloost::math::clamp(r*255.0, 0.0, 255.0);
  color.rgbReserved   = (unsigned int) gloost::math::clamp(a*255.0, 0.0, 255.0);

  FreeImage_FillBackground(_freeImage_bitmap, &color, FI_COLOR_IS_RGBA_COLOR);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   flips an Image horizontally
  \remarks ...
*/

void
Image::flipH()
{
  FreeImage_FlipHorizontal(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   flips an Image vertically
  \remarks ...
*/

void
Image::flipV()
{
  FreeImage_FlipVertical(_freeImage_bitmap);
}


////////////////////////////////////////////////////////////////////////////////


///**
//  \brief   rotates the image around its center
//  \remarks ...
//*/
//
//void
//Image::rotate(mathType angleInDegrees)
//{
//  FreeImage_RotateEx(_freeImage_bitmap, angleInDegrees, 0, 0, _width*0.5, _height*0.5, true);
//}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   scales an Image
  \remarks ...
*/

//void
//Image::scale(unsigned width, unsigned height, FREE_IMAGE_FILTER filter)
//{
//  FreeImage_Rescale(_freeImage_bitmap, width, height, filter);
//  _width  = width;
//  _height = height;
//}


//////////////////////////////////////////////////////////////////////////////

/**
  \brief   Sets the color of pixel x,y
  \param   ...
  \remarks ...
*/

void
Image::setPixel(unsigned int x,
                unsigned int y,
                mathType r,
                mathType g,
                mathType b,
                mathType a)
{
  // FreeImage uses BGRA scheme
  RGBQUAD color;
  color.rgbBlue       = (unsigned int) (b*255);
  color.rgbGreen      = (unsigned int) (g*255);
  color.rgbRed        = (unsigned int) (r*255);
  color.rgbReserved   = (unsigned int) (a*255);

  {
    FreeImage_SetPixelColor(_freeImage_bitmap, x, y, &color);
  }

//  FreeImage_F(_freeImage_bitmap, &color);
}


//////////////////////////////////////////////////////////////////////////////


void
Image::writeToPNG(const std::string& path)
{
  writeToPNG(gloost::PathType(path));
}


//////////////////////////////////////////////////////////////////////////////


void
Image::writeToPNG(const gloost::PathType& path)
{
  #ifdef GLOOST_WINDOWS
    FreeImage_SaveU(FIF_PNG,_freeImage_bitmap, path.wstring().c_str(), 0);
  #else
    FreeImage_Save(FIF_PNG,_freeImage_bitmap,  path.string().c_str(), 0);
  #endif

}


//////////////////////////////////////////////////////////////////////////////


void
Image::writeAsThumbnail(unsigned maxPixelSize, const std::string& path)
{
  FIBITMAP* bitmap = FreeImage_MakeThumbnail(_freeImage_bitmap, maxPixelSize);
  FreeImage_Save(FIF_PNG,bitmap,(char*)path.c_str(), 0);
}


//////////////////////////////////////////////////////////////////////////////


Image::shared_ptr
Image::getAsThumbnail(unsigned maxPixelSize)
{
  return shared_ptr(new Image(FreeImage_MakeThumbnail(_freeImage_bitmap, maxPixelSize)));
}


////////////////////////////////////////////////////////////////////////////////

} // namespace gloost


