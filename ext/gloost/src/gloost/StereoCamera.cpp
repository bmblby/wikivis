
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



// gloost includes
#include <gloost/StereoCamera.h>

namespace gloost
{

  ///////////////////////////////////////////////////////////////////////////////


  ///


  StereoCamera::StereoCamera(const Matrix& cyclopsmatrix,
                             float nearPlaneZ,
                             float farPlaneZ,
                             float eye_seperation,
                             float screenwidth,
                             float screenheight)
    : ScreenCamera(cyclopsmatrix,nearPlaneZ,farPlaneZ,screenwidth,screenheight),
      _eye_seperation(eye_seperation)
  {}


  ///////////////////////////////////////////////////////////////////////////////


  ///

  StereoCamera::StereoCamera(const Matrix& cyclopsmatrix,
                             float nearPlaneZ,
                             float farPlaneZ,
                             float eye_seperation,
                             const Matrix& screenmatrix,
                             float screenwidth,
                             float screenheight)
    : ScreenCamera(cyclopsmatrix,nearPlaneZ,farPlaneZ,screenmatrix,screenwidth,screenheight),
      _eye_seperation(eye_seperation)

{}


///////////////////////////////////////////////////////////////////////////////


  ///

StereoCamera::~StereoCamera()
{

}

///////////////////////////////////////////////////////////////////////////////


///

void
StereoCamera::setEyeSeperation(float eye_seperation)
{
  _eye_seperation = eye_seperation;
  setLeft();
}

///////////////////////////////////////////////////////////////////////////////


///

void
StereoCamera::setLeft()
{
  _eye.setIdentity();
  _eye.setTranslate(-_eye_seperation*0.5f,0.0,0.0);
  update();
  updateFrustum();
}

///////////////////////////////////////////////////////////////////////////////


///
void  StereoCamera::setRight()
{
  _eye.setIdentity();
  _eye.setTranslate(_eye_seperation*0.5f,0.0,0.0);
  update();
  updateFrustum();

}


///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
