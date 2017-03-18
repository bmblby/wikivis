
#ifndef H_GLOOST_FBO
#define H_GLOOST_FBO

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


/*

This source was taken from the http://www.gpgpu.org


FramebufferObject Class. This class encapsulates the FramebufferObject
(FBO) OpenGL spec. See the official spec at:
	http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt

for details.

A framebuffer object (FBO) is conceptually a structure containing pointers
to GPU memory. The memory pointed to is either an OpenGL texture or an
OpenGL RenderBuffer. FBOs can be used to render to one or more textures,
share depth buffers between multiple sets of color buffers/textures and
are a complete replacement for pbuffers.

Performance Notes:
  1) It is more efficient (but not required) to call Bind()
     on an FBO before making multiple method calls. For example:

      FramebufferObject fbo;
      fbo.Bind();
      fbo.AttachTexture(GL_TEXTURE_2D, texId0, GL_COLOR_ATTACHMENT0_EXT);
      fbo.AttachTexture(GL_TEXTURE_2D, texId1, GL_COLOR_ATTACHMENT1_EXT);
      fbo.IsValid();

    To provide a complete encapsulation, the following usage
    pattern works correctly but is less efficient:

      FramebufferObject fbo;
      // Note : No Bind() call
      fbo.AttachTexture(GL_TEXTURE_2D, texId0, GL_COLOR_ATTACHMENT0_EXT);
      fbo.AttachTexture(GL_TEXTURE_2D, texId1, GL_COLOR_ATTACHMENT1_EXT);
      fbo.IsValid();

    The first usage pattern binds the FBO only once, whereas
    the second usage binds/unbinds the FBO for each method call.

  2) Use FramebufferObject::Disable() sparingly. We have intentionally
     left out an "Unbind()" method because it is largely unnecessary
     and encourages rendundant Bind/Unbind coding. Binding an FBO is
     usually much faster than enabling/disabling a pbuffer, but is
     still a costly operation. When switching between multiple FBOs
     and a visible OpenGL framebuffer, the following usage pattern
     is recommended:

      FramebufferObject fbo1, fbo2;
      fbo1.Bind();
        ... Render ...
      // Note : No Unbind/Disable here...

      fbo2.Bind();
        ... Render ...

      // Disable FBO rendering and return to visible window
      // OpenGL framebuffer.
      FramebufferObject::Disable();
*/



// gloost system includes
#include <gloost/glErrorUtil.h>


// cpp includes
#include <GL/glew.h>
#include <iostream>


namespace gloost
{
namespace gl
{

  /// implementation of an frame buffer object

class Fbo
{
  public:

    /// class constructor
    Fbo();

    /// class destructor
    virtual ~Fbo();


    /// Bind this FBO as current render target
    void bind(unsigned int attachment = GL_COLOR_ATTACHMENT0);


    /// returns the OpenGL handle for this FBO
    unsigned int getFboHandle();


    /// Bind a texture to the "attachment" point of this FBO
    virtual void attachTexture( GLenum texTarget,
                                GLuint textureHandle,
                                GLenum attachment = GL_COLOR_ATTACHMENT0,
                                int mipLevel      = 0,
                                int zSlice        = 0 );


    /// Bind an array of textures to multiple "attachment" points of this FBO
    /// By default, the first 'numTextures' attachments are used,
    /// starting with GL_COLOR_ATTACHMENT0_EXT
    virtual void attachTextures( int numTextures,
                                 GLenum texTarget[],
                                 GLuint textureHandles[],
                                 GLenum attachment[] = NULL,
                                 int mipLevel[]      = NULL,
                                 int zSlice[]        = NULL );



    /// Bind a render buffer to the "attachment" point of this FBO
    virtual void attachRenderBuffer( GLuint buffId,
                                     GLenum attachment = GL_COLOR_ATTACHMENT0 );

    /// Bind an array of render buffers to corresponding "attachment" points
    /// of this FBO.
    /// - By default, the first 'numBuffers' attachments are used,
    ///   starting with GL_COLOR_ATTACHMENT0_EXT
    virtual void attachRenderBuffers( int numBuffers, GLuint buffId[],
                                      GLenum attachment[] = NULL );



    /// Free any resource bound to the "attachment" point of this FBO
    void unattach( GLenum attachment );

    /// Free any resources bound to any attachment points of this FBO
    void unattachAll();



    /// Is this FBO currently a valid render target?
    ///  - Sends output to std::cerr by default but can
    ///    be a user-defined C++ stream
    ///
    /// Note : This function works correctly in debug build
    ///        mode but always returns "true" if NDEBUG is
    ///        is defined (optimized builds)

    bool isValid( std::ostream& ostr = std::cerr );


    /// BEGIN : Accessors
      /// Is attached type GL_RENDERBUFFER_EXT or GL_TEXTURE?
      GLenum getAttachedType( GLenum attachment );

      /// What is the Id of Renderbuffer/texture currently
      /// attached to "attachement?"
      GLuint getAttachedId( GLenum attachment );

      /// Which mipmap level is currently attached to "attachement?"
      GLint  getAttachedMipLevel( GLenum attachment );

      /// Which cube face is currently attached to "attachment?"
      GLint  getAttachedCubeFace( GLenum attachment );

      /// Which z-slice is currently attached to "attachment?"
      GLint  getAttachedZSlice( GLenum attachment );
    /// END : Accessors


      // BEGIN : Static methods global to all FBOs
      /// Return number of color attachments permitted
      static int getMaxColorAttachments();


      /// Disable all FBO rendering and return to traditional, windowing-system controlled framebuffer
      static void unbind();

      // END : Static methods global to all FBOs

  protected:

    /// bind only if the binded id != this.m_fboId to avoid unnessesarry bindings
    void  guardedBind();
    void  guardedUnbind();

    /// Attache texture depending of its texTarget (GL_TEXTURE_1D, ..._2D, ..._3D, GL_TEXTURE_RECTANGLE_ARB, )
    void   attachTextureByTargetType( GLenum attachment, GLenum texTarget, GLuint texId, int mipLevel, int zSlice );
    GLuint generate();

  private:
    GLuint _fboHandle;
    GLint  _savedFboHandle;


};


} // namespace gl
} // namespace gloost


#endif // H_GLOOST_FBO

