
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



#ifndef GLOOST_CAMERA_H
#define GLOOST_CAMERA_H



// gloost includes
#include <gloost/Matrix.h>
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/Ray.h>
#include <gloost/Frustum.h>



namespace gloost
{

  /// camera basis class

class Camera
{

 public:

  /// class constructor
  Camera();
  Camera(const Matrix& projectionMatrix,
         const Matrix& modelViewMatrix);
  Camera(float fov,
         float aspectRatio,
         float nearPlaneZ,
         float farPlaneZ);


  /// destructor
  virtual ~Camera();


  ///
  const Matrix& getViewMatrix() const;
  ///
  virtual void setViewMatrix(const Matrix& viewMatrix);


  ///
  const Matrix& getProjectionMatrix() const;
  ///
  virtual void setProjectionMatrix(const Matrix& projectionMatrix);


  /// set the camera to the state
//  virtual void set();



  const Frustum& getFrustum() const;



  virtual Ray getPickRay(unsigned int /*mouseH*/,
                         unsigned int /*mouseV*/,
                         unsigned int /*screenWidth*/,
                         unsigned int /*screenHeight*/){ return Ray();};



  /// returns position of the camera !!! Works only if position was set via lookAt(Point3 ...)
  virtual const Point3& getPosition();



 protected:

  virtual void updateProjection(){};
  virtual void updateFrustum(){};


  Point3 _cameraPosition;

  float _fov;
  float _aspectRatio;
  float _nearPlaneZ;
  float _farPlaneZ;

  bool _projection_dirty;

  Matrix  _projection;
  Matrix  _viewMatrix;
  Frustum _frustum;
};


} // namespace gloost


#endif // GLOOST_CAMERA_H
