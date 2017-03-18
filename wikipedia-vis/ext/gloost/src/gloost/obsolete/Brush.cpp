
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
  author: 2008 by Felix Weiﬂig
  mail:   thesleeper@gmx.net
*/

// gloost includes
#include <gloost/gl/Texture.h>
#include <gloost/obsolete/gl/gloostRenderGoodies.h>
#include <gloost/obsolete/Brush.h>

// cpp includes
#include <string>
#include <iostream>
#include <math.h>

namespace gloost
{

///////////////////////////////////////////////////////////////////////////////

  /// class constructor

Brush::Brush(const std::string filePath, unsigned int width, unsigned int height):
  	_last_u(0.0),
	  _last_v(0.0),
	  _scale(0.3),
	  _rotation(0.0),
	  _opacity(1.0),
	  _flipH(false),
	  _flipV(false),
	  _seamless(false),
	  _rotateToPath(false),
	  _vibration(0.0),
	  _brushTextureId(0),
	  _brushTexture(0),
	  _transferTextureId(0),
	  _transferTexture(0),
	  _texManager(gloost::TextureManager::getInstance()),
	  _fbo(0),
	  _internalWidth(width),
	  _internalHeight(height)
{


	// load the brush texture
	_brushTextureId = _texManager->createTexture(filePath);

	// get Texture pointer for fast access
	_brushTexture = _texManager->getTextureWithoutRefcount(_brushTextureId);

	// initialise transfer texture
	_transferTexture = new gl::Texture(_internalWidth,
                                     _internalHeight,
                                     16,
                                     GL_TEXTURE_2D,
                                     GL_RGBA,
                                     GL_RGBA,
                                     GL_FLOAT);

	_transferTextureId = _texManager->addTexture(_transferTexture);

	// init fbo to render to
	_fbo = new gl::Fbo();

	// assign transfer texture to the fbo to render the brush into it
	_fbo->attachTexture(GL_TEXTURE_2D,
                      _transferTexture->getTextureHandle(),
                      GL_COLOR_ATTACHMENT0);

  gl::Fbo::unbind();
}

///////////////////////////////////////////////////////////////////////////////

  /// class constructor

Brush::~Brush()
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

  /// initialise Brush

void
Brush::init()
{

}

///////////////////////////////////////////////////////////////////////////////

void
Brush::clearBrushTexture()
{

  // Render to the FBO
  _fbo->bind();

  // clear
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _fbo->unbind();
}

///////////////////////////////////////////////////////////////////////////////

  /// renders the brush into a texture

void
Brush::renderBrushToTransferTexture(float pos_u, float pos_v)
{
  // rotate to path
  float curr_rotation = _rotation;

  glPushMatrix();
  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    {

      glViewport(0, 0, _internalWidth, _internalHeight);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-0.5, 0.5, -0.5, 0.5, 0.1, 10.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0.0, 0.0, 5.0,
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0);

      glDisable(GL_COLOR_MATERIAL);

      // Render to the FBO
      _fbo->bind();

      // color
      glColor4f(_opacity, _opacity, _opacity, 1.0);

      // bind brush texture
      _brushTexture->bind();

      if (_seamless)
      {
        // draw 9 brushes, with constant distance, to the brushToTexture FBO
        glPushMatrix();
        {
          glTranslatef(-0.5+pos_u, 0.5-pos_v, 0);

          for (int x=-1; x!=2; ++x)
            for (int y=-1; y!=2; ++y)
            {
              glPushMatrix();
                glTranslatef(x, y, 0);
                glScalef(_scale, _scale, _scale);
                glRotatef(curr_rotation, 0.0, 0.0, 1.0);
                gl::drawQuadCenter();
              glPopMatrix();
            }
        }
        glPopMatrix();
      }
      else
      {
        // draw only one brush on uv position
        glPushMatrix();
        {
          glTranslatef(-0.5+pos_u, 0.5-pos_v, 0);
          glScalef(_scale, _scale, _scale);
          glRotatef(curr_rotation, 0.0, 0.0, 1.0);
          gl::drawQuadCenter();
        }
        glPopMatrix();
      }

      _brushTexture->unbind();

      gl::Fbo::unbind();
    }
    glPopAttrib();
  }
  glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////

  /// returns the textureManager id, for the texture the brush was rendered to

unsigned int
Brush::getTransferTextureId()
{
  return _transferTextureId;
}

///////////////////////////////////////////////////////////////////////////////

  /// returns a pointer of the texture, the brush was rendered to (no refCount!!!)

gl::Texture*
Brush::getTransferTexture()
{
  return _transferTexture;
}

///////////////////////////////////////////////////////////////////////////////

  /// get scale

float
Brush::getScale()
{
  return _scale;
}

///////////////////////////////////////////////////////////////////////////////

  /// set scale

void
Brush::setScale(float scale)
{
  _scale = scale;
}

///////////////////////////////////////////////////////////////////////////////

  /// get rotation in degrees

float
Brush::getRotation()
{
  return _rotation;
}

///////////////////////////////////////////////////////////////////////////////

  /// set rotation in degrees

void
Brush::setRotation(float rotation)
{
  _rotation = rotation;
}

///////////////////////////////////////////////////////////////////////////////

  /// get opacity (0.0 ... 1.0)

float
Brush::getOpacity()
{
  return _opacity;
}

///////////////////////////////////////////////////////////////////////////////

  /// set opacity (0.0 ... 1.0)

void
Brush::setOpacity(float opacity)
{
  _opacity = opacity;
}

///////////////////////////////////////////////////////////////////////////////

  /// get flipH

bool
Brush::getFlipH()
{
  return _flipH;
}

///////////////////////////////////////////////////////////////////////////////

  /// set flipH

void
Brush::setFlipH(bool flipH)
{

  _flipH = flipH;

}

///////////////////////////////////////////////////////////////////////////////

  /// get flipV

bool
Brush::getFlipV()
{
  return _flipV;
}

///////////////////////////////////////////////////////////////////////////////

  /// set flipV

void
Brush::setFlipV(bool flipV)
{

  _flipV = flipV;

}

///////////////////////////////////////////////////////////////////////////////

  /// get seamless

bool
Brush::getSeamless()
{
  return _seamless;
}

///////////////////////////////////////////////////////////////////////////////

  /// if set to 1, the brush texture will be seamless

void
Brush::setSeamless(bool seamless)
{
  _seamless = seamless;
}

///////////////////////////////////////////////////////////////////////////////


  /// get RotateToPath

bool
Brush::getRotateToPath()
{
  return _rotateToPath;
}

///////////////////////////////////////////////////////////////////////////////

  /// if set to 1, brush will rotate itsself to the moving direction

void
Brush::setRotateToPath(bool rotateToPath)
{
  _rotateToPath = rotateToPath;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace gloost


