
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



#ifndef H_GLOOST_PERSPECTIVECAMERA
#define H_GLOOST_PERSPECTIVECAMERA



// gloost includes
#include <gloost/Camera.h>
#include <gloost/Ray.h>



namespace gloost
{


  ///  perspective camera

  class PerspectiveCamera : public Camera
  {

  public:


    /// class constructor
    PerspectiveCamera();
    PerspectiveCamera(float fov, float aspect, float nearPlaneZ, float farPlaneZ);
    PerspectiveCamera(const PerspectiveCamera& perpCamera);

    /// class destructor
    virtual ~PerspectiveCamera();



    /// set/get field of view
    void setFov(float fov);
    float getFov() const;

    /// set/get aspect ratio
    void setAspectRatio(float aspectRatio);
    float getAspectRatio() const;

    /// set/get distance to the near plane
    void setNear(float nearPlaneZ);
    float getNear() const;

    /// set/get distance to the far plane
    void setFar(float farPlaneZ);
    float getFar() const;


    ///
    virtual void setViewMatrix(const Matrix& viewMatrix);
    ///
    virtual void setProjectionMatrix(const Matrix& projectionMatrix);


    /// set poisition, point to look at and up vector of the camera
    void lookAt(const Point3&  cameraPosition,
                const Point3&  pointToLookAt,
                const Vector3& upVector);


    /// returns position of the camera !!! Works only if position was set via lookAt(Point3 ...)
    const Point3& getPosition();

//    virtual void set();

    virtual Ray getPickRay( unsigned int screenWidth,
                            unsigned int screenHeight,
                            unsigned int mouseH,
                            unsigned int mouseV);

  protected:



    /*virtual*/ void updateProjection();
    /*virtual*/ void updateFrustum();
};


} // namespace gloost


#endif // GLOOST_PERSPECTIVECAMERA_H
