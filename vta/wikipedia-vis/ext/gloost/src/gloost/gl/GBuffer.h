
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



#ifndef H_GLOOST_GBUFFER
#define H_GLOOST_GBUFFER



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/UniformTypes.h>
#include <gloost/Image.h>


// cpp includes
#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>


namespace gloost
{
namespace gl
{

  class Fbo;
  class Texture;


  //  GBuffer, uses gloost::Texture as attachments

class GBuffer
{
	public:

    /// a std::shared_ptr of an RendererBase instance
    typedef std::shared_ptr<GBuffer>       shared_ptr;
    typedef std::shared_ptr<const GBuffer> const_shared_ptr;


    // class constructor
    static shared_ptr create(unsigned width, unsigned height, unsigned internalFormat = GL_RGBA);

    // class destructor
	  virtual ~GBuffer();


    // binds the FBO with a set of attachements
    void bind(const std::vector<GLenum> attachements = {GL_COLOR_ATTACHMENT0});


    // unbinds the FBO
    void unbind();



	  // adds a texture to the GBuffer
	  gloostId addTexture(unsigned minMagFilterMode = GL_LINEAR);

	  // returns a vector of texture ids
	  std::vector<gloostId>& getTextureGids();

	  // returns the gloost::Texture* of a color attachment
	  Texture* getAttachedTexture(gloostId attachmentGid = 0);

	  // returns the gloost::Texture* of a color attachment
	  Texture* getDepthAttachmentTexture();

	  // returns the gloostId of the depth attachments gloost::Texture*
	  gloostId getDepthAttachmentGid();


	  // creates a depth buffer and attaches it to the FBO
	  void createDepthBuffer(unsigned internalFormat=GL_DEPTH_COMPONENT16);


	  // applies the viewport from 0,0 to width, height of this GBuffer
	  void applyViewport() const;



	  // returns width in pixel
	  unsigned getWidth() const;

	  // returns height in pixel
	  unsigned getHeight() const;


    // picks one value of a color attachment of an GBuffer
	  vec4 pickBuffer(gloostId attachmentGid,
                    unsigned         pickPosH,
                    unsigned         pickPosV);


    // writes the buffer to a RGB32F raw file
    bool writeBufferToRawFileRGBA32F(gloostId attachmentGid, const std::string& imageFilePath);

    // write the buffer to a png file
    bool writeBufferToPNG(gloostId attachmentGid, const std::string& imageFilePath);

    std::shared_ptr<gloost::Image> getAsGloostImage(gloostId attachmentGid);



    // returns the fbo
	  Fbo* getFbo();


	protected:

   unsigned _width;
   unsigned _height;
   unsigned _internalFormat;

   Fbo* _fbo;

   std::vector<gloostId> _textureGids;
   gloostId              _depthBufferTextureGid;


    // class constructor
    GBuffer(unsigned width, unsigned height, unsigned internalFormat);

	private:

   // ...

};


} // namespace gl
} // namespace gloost


#endif // H_GLOOST_GBUFFER
