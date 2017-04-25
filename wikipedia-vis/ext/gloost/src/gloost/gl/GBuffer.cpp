
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



// contrib includes
#include <gloost/gl/GBuffer.h>

// gloost system includes
#include <gloost/TextureManager.h>
#include <gloost/gl/Texture.h>
#include <gloost/gl/Fbo.h>
#include <gloost/BinaryFile.h>


// cpp includes
#include <string>
#include <iostream>
#include <assert.h>



namespace gloost
{

namespace gl
{


/**
  \class   GBuffer

  \brief   Stores gloost::Textures of same size within a gloost::Fbo.
           Provides factory methodes for creating attachments

  \author  Felix Weiszig
  \date    April 2013
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

GBuffer::GBuffer(unsigned width, unsigned height, unsigned internalFormat):
    _width(width),
    _height(height),
    _internalFormat(internalFormat),
    _fbo(new Fbo()),
    _textureGids(),
    _depthBufferTextureGid(0)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

GBuffer::~GBuffer()
{
	delete _fbo;

	for (unsigned i=0; i!=_textureGids.size(); ++i)
	{
	  TextureManager::get()->dropReference(_textureGids[i]);
	}

	if (_depthBufferTextureGid)
  {
	  TextureManager::get()->dropReference(_depthBufferTextureGid);
  }

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   binds the FBO with a set of attachements
  \param   ...
  \remarks ...
*/

void
GBuffer::bind(const std::vector<GLenum> attachements)
{
  _fbo->bind();
  glDrawBuffers(attachements.size(), &attachements.front());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   unbinds the FBO
  \param   ...
  \remarks ...
*/

void
GBuffer::unbind()
{
  _fbo->unbind();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a texture to the GBuffer
  \param   ...
  \remarks ...
*/

gloostId
GBuffer::addTexture(unsigned minMagFilterMode)
{
  Texture* buffer = new Texture(_width,
                                                _height,
                                                16,
                                                GL_TEXTURE_2D,
                                                _internalFormat);

  buffer->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP);
  buffer->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP);
  buffer->setTexParameter(GL_TEXTURE_MIN_FILTER, minMagFilterMode);
  buffer->setTexParameter(GL_TEXTURE_MAG_FILTER, minMagFilterMode);

  _fbo->attachTexture( GL_TEXTURE_2D,
                       buffer->getTextureHandle(),
                       GL_COLOR_ATTACHMENT0+_textureGids.size());

  const auto id = TextureManager::get()->addTexture(buffer);
  _textureGids.push_back(id);

  _fbo->unbind();

  return id;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a texture to the GBuffer
  \param   ...
  \remarks ...
*/

/*static */
GBuffer::shared_ptr
GBuffer::create(unsigned width, unsigned height, unsigned internalFormat)
{
  return shared_ptr(new GBuffer(width, height, internalFormat));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a vector of texture ids
  \param   ...
  \remarks ...
*/

std::vector<gloostId>&
GBuffer::getTextureGids()
{
	return _textureGids;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Texture* of a color attachment
  \param   attachmentGid number of the color attachment as an int
  \remarks ...
*/

Texture*
GBuffer::getAttachedTexture(gloostId attachmentGid)
{
  if ((size_t)attachmentGid < _textureGids.size())
  {
    return TextureManager::get()->getTextureWithoutRefcount(_textureGids[attachmentGid]);
  }

	return 0;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Texture* of a depth attachment
  \param
  \remarks ...
*/

Texture*
GBuffer::getDepthAttachmentTexture()
{
  if (_depthBufferTextureGid)
  {
    return TextureManager::get()->getTextureWithoutRefcount(_depthBufferTextureGid);
  }

	return 0;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the gloostId of the depth attachments Texture*
  \param
  \remarks ...
*/

gloostId
GBuffer::getDepthAttachmentGid()
{
	return _depthBufferTextureGid;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a depth buffer and attaches it to the FBO
  \remarks ...
*/

void
GBuffer::createDepthBuffer(unsigned internalFormat)
{
  // since we need depth test while rendering the scene, we also attach a depth texture
  Texture* depthBuffer = new Texture(_width,
                                     _height,
                                     4,
                                     GL_TEXTURE_2D,
                                     internalFormat,
                                     GL_DEPTH_COMPONENT,
                                     GL_FLOAT);
  depthBuffer->setTexParameter( GL_TEXTURE_COMPARE_MODE, GL_NONE );

//  depthBuffer->setTexParameter( GL_DEPTH_TEXTURE_MODE,   GL_ALPHA );
  depthBuffer->setTexParameter (GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

  // Attache to fbo
  _fbo->attachTexture(GL_TEXTURE_2D,
                      depthBuffer->getTextureHandle(),
                      GL_DEPTH_ATTACHMENT);

  _depthBufferTextureGid = TextureManager::get()->addTexture(depthBuffer);

  Fbo::unbind();
}



////////////////////////////////////////////////////////////////////////////////


/**
  \brief  applies the viewport from 0,0 to width,height of this GBuffer
  \param   ...
  \remarks ...
*/

void
GBuffer::applyViewport() const
{
	glViewport(0, 0, _width, _height);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief  returns width in pixel
  \param   ...
  \remarks ...
*/

unsigned
GBuffer::getWidth() const
{
	return _width;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief  returns height in pixel
  \param   ...
  \remarks ...
*/

unsigned
GBuffer::getHeight() const
{
	return _height;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the fbo
  \param   ...
  \remarks ...
*/

Fbo*
GBuffer::getFbo()
{
	return _fbo;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief  picks one value of a color attachment of an GBuffer
  \param   ...
  \remarks G-Buffer attachment will be bound and unbound within this methode, be sure to have your
           GL state in order...
*/

vec4
GBuffer::pickBuffer(gloostId attachmentGid,
                    unsigned         pickPosH,
                    unsigned         pickPosV)
{

  assert((size_t)attachmentGid < _textureGids.size()); // request an attachment which is not present

	vec4 texelValue        = {0.0f, 0.0f, 0.0f, 0.0f};
	Texture* bufferTexture = TextureManager::get()->getTextureWithoutRefcount(_textureGids[attachmentGid]);

	_fbo->bind(GL_COLOR_ATTACHMENT0+attachmentGid);
  {
    glReadBuffer(GL_COLOR_ATTACHMENT0+attachmentGid);
    glReadPixels(pickPosH,  pickPosV,
                 1,  1,
                 bufferTexture->getPixelFormat(),
                 GL_FLOAT,
                 &texelValue);
  }
	_fbo->unbind();

  return texelValue;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief  writes the buffer to a RGBA32F raw file (to be imported by Gimp for example) + a meta file as txt
  \param   ...
  \remarks G-Buffer attachment will be bound and unbound within this methode, be sure to have your
           GL state in order...
*/

bool
GBuffer::writeBufferToRawFileRGBA32F(gloostId attachmentGid, const std::string& imageFilePath)
{

  assert((size_t)attachmentGid < _textureGids.size());

//	Texture* bufferTexture = TextureManager::get()->getTextureWithoutRefcount(_textureGids[attachmentGid]);

  std::vector<unsigned char> values(_width*_height*4);

	_fbo->bind(GL_COLOR_ATTACHMENT0+attachmentGid);
  {
    glReadBuffer(GL_COLOR_ATTACHMENT0+attachmentGid);
    glReadPixels(0,  0,
                 _width,  _height,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &values.front());
  }
	_fbo->unbind();


  BinaryFile::write(imageFilePath, (unsigned char*)&values.front(),values.size()*sizeof(unsigned char));

  gloost::BinaryFile statFile;
  statFile.openToWrite(imageFilePath + "_stat.txt");
  statFile.writeString( gloost::helper::toString(_width) + " " + gloost::helper::toString(_height) + "\n" );
  statFile.writeString( "RGBA, 8 bit/color" );
  statFile.writeString( "\n" );
  statFile.close();

  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief  writes the buffer to a RGBA PNG file
  \param   ...
  \remarks G-Buffer attachment will be bound and unbound within this methode, be sure to have your
           GL state in order...
*/

bool
GBuffer::writeBufferToPNG(gloostId attachmentGid, const std::string& imageFilePath)
{

  assert((size_t)attachmentGid < _textureGids.size());

//	Texture* bufferTexture = TextureManager::get()->getTextureWithoutRefcount(_textureGids[attachmentGid]);

  std::vector<unsigned char> values(_width*_height*4);

	_fbo->bind(GL_COLOR_ATTACHMENT0+attachmentGid);
  {
    glReadBuffer(GL_COLOR_ATTACHMENT0+attachmentGid);
    glReadPixels(0,  0,
                 _width,  _height,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &values.front());
  }
	_fbo->unbind();

	gloost::helper::writeRGBAToPNG(&values.front(), _width, _height, false, imageFilePath);

  return true;
}



////////////////////////////////////////////////////////////////////////////////


std::shared_ptr<gloost::Image>
GBuffer::getAsGloostImage(gloostId attachmentGid)
{

  assert((size_t)attachmentGid < _textureGids.size());

//	Texture* bufferTexture = TextureManager::get()->getTextureWithoutRefcount(_textureGids[attachmentGid]);

  std::vector<unsigned char> values(_width*_height*4);

	_fbo->bind(GL_COLOR_ATTACHMENT0+attachmentGid);
  {
    glReadBuffer(GL_COLOR_ATTACHMENT0+attachmentGid);
    glReadPixels(0,  0,
                 _width,  _height,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &values.front());
  }
	_fbo->unbind();

	return gloost::Image::create(_width, _height, 32, &values.front());
}


////////////////////////////////////////////////////////////////////////////////



} // namespace gl
} // namespace gloost


