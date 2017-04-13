
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


#ifndef H_GLOOST_BRUSH
#define H_GLOOST_BRUSH



// gloost system includes
#include <gloost/TextureManager.h>
#include <gloost/gl/Fbo.h>


// cpp includes
#include <string>



namespace gloost
{

namespace gl { class Texture; }

  ///  Draws a sprite into a texture wich can be used as an input for a shader to manipulate local values

class Brush
{
	public:

    /// class constructor
    Brush(const std::string filePath, unsigned int width = 512, unsigned int height = 512);

    /// class destructor
	  ~Brush();


    /// clear brush textur clearBrushTexture
    void clearBrushTexture();

	  /// renders the brush into a texture
	  void renderBrushToTransferTexture(float pos_u, float pos_v);


	  /// returns the textureManager id, for the texture the brush was rendered to
	  unsigned int getTransferTextureId();


	  /// returns a pointer of the texture, the brush was rendered to (no refCount!!!)
	  gl::Texture* getTransferTexture();


	  /// get scale
	  float getScale();
	  /// set scale
	  void setScale(float scale);


	  /// get rotation in degrees
	  float getRotation();
	  /// set rotation in degrees
	  void setRotation(float rotation);


	  /// get opacity (0.0 ... 1.0)
	  float getOpacity();
	  /// set opacity (0.0 ... 1.0)
	  void  setOpacity(float opacity);


	  /// get flipH
	  bool getFlipH();
	  /// set flipH
	  void setFlipH(bool flipH);


	  /// get flipV
	  bool getFlipV();
	  /// set flipV
	  void setFlipV(bool flipV);


	  /// get seamless
	  bool getSeamless();
	  /// if set to 1, the brush texture will be seamless
	  void setSeamless(bool seamless);


	  /// get RotateToPath
	  bool getRotateToPath();
	  /// if set to 1, brush will rotate itsself to the moving direction
	  void setRotateToPath(bool rotateToPath);


	  /// get Vibration (0.0 ... 1.0)
	  float getVibration();
	  /// set Vibration (0.0 ... 1.0)
	  void  setVibration(float vibration);



	private:

	  // all the brush properties
	  float _last_u;
	  float _last_v;

	  float _scale;
	  float _rotation;
	  float _opacity;
	  bool  _flipH;
	  bool  _flipV;
	  bool  _seamless;
	  bool  _rotateToPath;
	  float _vibration;


	  /// id of the brush texture within the textureManager
	  unsigned int _brushTextureId;

	  /// pointer to the texture of the brush
	  gl::Texture* _brushTexture;


	  /// pointer to the transfer texture id within the textureManager
	  unsigned int _transferTextureId;

	  /// pointer to the transfer texture
	  gl::Texture* _transferTexture;


	  /// pointer to the textureManager
	  TextureManager* _texManager;


	  /// fbo to render the brush to the brush texture
	  gl::Fbo* _fbo;


	  // size of the transferTexture
	  unsigned int _internalWidth;
	  unsigned int _internalHeight;


	  /// initialise Brush
	  void init();

};


} // namespace gloost


#endif // H_GLOOST_BRUSH


