
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



#ifndef GLOOST_STEREOCAMERA_H
#define GLOOST_STEREOCAMERA_H



// gloost includes
#include <gloost/ScreenCamera.h>



namespace gloost
{


  ///  StereoCamera

  class StereoCamera : public ScreenCamera
  {

  public:


    /// class constructors

    StereoCamera(const Matrix& cyclopsmatrix,
                 float nearPlaneZ,
                 float farPlaneZ,
                 float eye_seperation,
                 float screenwidth,
                 float screenheight);

    StereoCamera(const Matrix& cyclopsmatrix,
                 float nearPlaneZ,
                 float farPlaneZ,
                 float eye_seperation,
                 const Matrix& screenmatrix,
                 float screenwidth,
                 float screenheight);

    /// class destructor
    ~StereoCamera();

    void setEyeSeperation(float eye_seperation);

    void setLeft();

    void setRight();



 private:

    float _eye_seperation;


};


} // namespace gloost


#endif // GLOOST_STEREOCAMERA_H
