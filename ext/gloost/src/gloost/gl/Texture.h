
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



#ifndef GLOOST_TEXTURE_H
#define GLOOST_TEXTURE_H



// gloost system includes
#include <gloost/gl/MultiGlContextEntity.h>
#include <gloost/Matrix.h>
#include <gloost/UniformTypes.h>
#include <gloost/BitMask.h>
#include <gloost/Image.h>
#include <gloost/BinaryBundle.h>
#include <gloost/gloostConfig.h>



// cpp includes
#include <string>
#include <map>
#include <GL/glew.h>


namespace gloost
{

namespace gl
{

  class Tbo;

///////////////////////////////////////////////////////////////////////////////


	/// Wrapper for an OpenGL texture

class Texture : public MultiGlContextEntity
{

	public:

    /// takes a filename and reads 8, 24 and 32bit images automatically
    Texture(const std::string& fileName);

    /// takes a filename and reads 8, 24 and 32bit images automatically
    Texture(const gloost::PathType& fileName);


    /// takes a filename and reads 8, 24 and 32bit images automatically
    Texture(const Image::shared_ptr& image);


	  /// class constructor
		Texture( unsigned width,
             unsigned height,
             unsigned bytesPerPixel,
		         GLenum target        = GL_TEXTURE_2D,
		         GLint internalFormat = GL_RGBA,
		         GLenum pixelFormat   = GL_RGBA,
		         GLenum pixelType     = GL_FLOAT);


	  /// class constructor for custom pixeldata
		Texture( unsigned width,
             unsigned height,
             unsigned depth,
             const unsigned char* pixeldata,
             unsigned bytesPerPixel,
		         GLenum   target,
		         GLint    internalFormat,
		         GLenum   pixelFormat,
		         GLenum   pixelType);


	  /// class constructor if Texture is used for a TEXTURE_BUFFER
		Texture(const std::shared_ptr<BinaryBundle>& data,
            unsigned internalFormat,
            unsigned usage  = GL_DYNAMIC_DRAW,
            unsigned target = GL_TEXTURE_BUFFER_EXT);


		/// class destructor
		virtual ~Texture();


		/// set a glTexParameter for this texture to be applied when texture is gernerated
		void setTexParameter(GLenum propertyName, float value);

		/// get TexParameter
		float getTexParameter(GLenum propertyName);

		/// remove a TexParameter from this texture to use GL default value
		void removeTexParameter(GLenum propertyName);

		/// get a map with all Texparameter for this texture
		std::map<GLenum, float>& getTexParameterMap();



		/// get width of the texture
		unsigned int getWidth() const;

		/// get height of the texture
		unsigned int getHeight() const;


		/// get the aspect ratio of the texture ( ar = width/height)
		float getAspecRatio() const;



		/// get gl's texture target for this texture (GL_TEXTURE_2D, GL_TEXTURE_3D, ...)
    GLenum getTarget();

    /// get gl's internal format for this texture
    GLint getInternalFormat();

    /// set internal format (GL_RGB, GL_RGBA, GL_RGBA32F, GL_ALPHA, GL_DEPTH_COMPONENT32, ...)
    void setInternalFormat(GLint internalFormat);

    /// get gl's pixel format for this texture
    GLenum getPixelFormat();

    /// get gl's pixel type for this texture
    GLenum getPixelType();



    /// get the GL texture handle
		unsigned int getTextureHandle(unsigned contextId = 0u);


   /// ...
   void freeHostData();



		/// writes the surfaces to a jpeg file (may be upside down)
//		void writeToFile(std::string);


		/// returns the  program side color value of the pixel with index i
		vec4 getPixelColor(unsigned int index);

		/// returns the  program side color value of the pixel position x,y
		vec4 getPixelColor (int x , int y );



		/// set the dirty flag to signal texture changes
		void setDirty();

		/// unset the dirty flag to signal that texture is up to date
		void setClean(unsigned contextId = 0u);

		/// check if the texture was changed to any time
		bool isDirty(unsigned contextId = 0u) const;



    /// generates a Gl texture from this texture
    /*virtual*/ bool initInContext(unsigned contextId = 0u);

	  /// remove the texture from a context
	  /*virtual*/ void removeFromContext(unsigned contextId = 0u);



		/// bind Texture to GL state
		void bind(unsigned int texUnitId=GL_TEXTURE0, unsigned contextId = 0u);

		///unbind Texture
		void unbind(unsigned contextId = 0u);



		/// reads the texture from the gfx-card back into the hostside buffer
		void readBack();

		/// copys the current framebuffer into the surface
//		void copyFramebuffer();



    /// set the texture matrix
    void setTextureMatrix(const Matrix& matrix);

    /// get the texture matrix
    const Matrix& getTextureMatrix();

    /// use the textureMatrix
    void enableTextureMatrix(bool trueOrFalse = true);

    /// returns 1 if texture matrix is enabled
    bool isTextureMatrixEnabled();


    /// use Mipmapping (don't work if attachet to an Fbo)
    void enableMipmaps(bool trueOrFalse = true);

    /// returns true if Mipmapping is enabled
    bool isMipmapsEnabled() const;



    /// set this texture shared or unshared (this flag is used by the TextureManager)
    void setShared(bool shared);

    /// returns 1 if this texture is shared within the TextureManager
    bool isShared();



    /// returns a pointer to the pixel data
    unsigned char* getPixels();

    /// returns a pointer to the pixel data within a BinaryBundle
    BinaryBundle::shared_ptr getPixelBundle();

    /// replaces pixeldata on host and server side, new pixels must have same size and format as old ones!
    void updatePixels(unsigned char* pixelbuffer,
                      unsigned int   lengthInByte);



    /// get the filename of this texture
    const gloost::PathType& getFilePath() const;

    /// get the filename of this texture
    const std::string getFileName() const;


	protected:



		/// width of the texture
		unsigned int _width;

		/// width of the texture
		unsigned int _height;

		/// depth of the (3D)texture
		unsigned int _depth;


	  /// texture target (GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, ...)
	  GLenum _target;

	  /// internal format of the texture (GL_RGB, GL_RGBA, GL_ALPHA, GL_DEPTH_COMPONENT32, ...)
	  GLint _internalFormat;

	  /// format of the pixeldata (GL_COLOR_INDEX, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA)
	  GLenum _pixelFormat;

	  /// Specifies the	type of the pixel data (GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, and GL_FLOAT)
		GLenum _pixelType;



    /// determines if the texture is up to date
    gloost::BitMask _dirty;


		/// texture Unit Id, given with the bind(id) methode, used for unbind
		std::vector<unsigned int> _textureUnitIdsForContexts;


		/// determines if this texture is using mipmapping
		bool _mipmapsEnabled;


		/// pixeldata if exiting
		BinaryBundle::shared_ptr _pixelData;

		// num bytes per pixel
		unsigned _bytesPerPixel;


    /// map stores all texparameters for this textures to be applied when texture is generated
    std::map<GLenum, float> _glTexParameters;


    /// texture matrix
    Matrix _texMatrix;

    /// use texture matrix
    bool _useTextureMatrix;

    /// flag is true if this texture is shared
    bool _shared;

    /// filename of this texture if it was loaded
    gloost::PathType _filePath;


    /// texture buffer object if target is GL_TEXTURE_BUFFER
    Tbo* _tbo;


    /// generates a Gl texture from this texture
    unsigned int generate( unsigned int width,
                           unsigned int height,
                           unsigned int depth     = 0,
                           GLenum target          = GL_TEXTURE_2D,
                           GLint internalFormat   = GL_RGBA,
                           GLenum pixelFormat     = GL_RGBA,
                           GLenum pixelType       = GL_FLOAT,
                           unsigned contextId = 0u);

};


  // ostream operator
  extern std::ostream& operator<< (std::ostream&, const Texture& );


}  // namespace gl
}  // namespace gloost


#endif // #ifndef GLOOST_TEXTURE_H
