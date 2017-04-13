
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
#include <gloost/gloostConfig.h>
#include <gloost/Camera.h>
#include <gloost/gl/gloostGlUtil.h>

// cpp includes
//#include <GL/glew.h>

namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Camera::Camera():
    _cameraPosition(Point3(0.0,0.0,0.0)),
    _fov(45),
    _aspectRatio(1.6),
    _nearPlaneZ(0.1),
    _farPlaneZ(100.0),
    _projection_dirty(true),
    _projection(),
    _viewMatrix(),
    _frustum()
{

}

///////////////////////////////////////////////////////////////////////////////

  /// class constructor

Camera::Camera(const Matrix& projectionMatrix, const Matrix& viewMatrix):
    _cameraPosition(Point3(0.0,0.0,0.0)),
    _fov(45.0),
    _aspectRatio(1.6),
    _nearPlaneZ(0.0),
    _farPlaneZ(0.0),
    _projection_dirty(true),
    _projection(projectionMatrix),
    _viewMatrix(viewMatrix),
    _frustum()
{

}

///////////////////////////////////////////////////////////////////////////////

  ///

Camera::Camera(float fov, float aspect, float nearPlaneZ, float farPlaneZ):
    _cameraPosition(Point3(0.0,0.0,0.0)),
    _fov(fov),
    _aspectRatio(aspect),
    _nearPlaneZ(nearPlaneZ),
    _farPlaneZ(farPlaneZ),
    _projection_dirty(true),
    _projection(),
    _viewMatrix(),
    _frustum()
{

}

///////////////////////////////////////////////////////////////////////////////

  ///

/* virtual */
Camera::~Camera()
{

}

///////////////////////////////////////////////////////////////////////////////

  ///

/*virtual*/
void Camera::setProjectionMatrix(const Matrix& projectionMatrix)
{
  _projection = projectionMatrix;
}

///////////////////////////////////////////////////////////////////////////////

  ///

/*virtual*/
void Camera::setViewMatrix(const Matrix& viewMatrix)
{
  _viewMatrix = viewMatrix;
}

///////////////////////////////////////////////////////////////////////////////

  ///
#if 0
/* virtual */
void Camera::set()
{
  std::cerr << std::endl << "D'OH from  Camera::set(): ";
  std::cerr << std::endl << "           This is deprecated and should NEVER be called...NEVER";
  glMatrixMode(GL_PROJECTION);
  gloostLoadMatrix(_projection.data());

  glMatrixMode(GL_MODELVIEW);
  gloostLoadMatrix(_viewMatrix.data());
}
#endif

///////////////////////////////////////////////////////////////////////////////

  ///

const Frustum&
Camera::getFrustum() const{
  return _frustum;
}

///////////////////////////////////////////////////////////////////////////////

  ///

const Matrix&
Camera::getProjectionMatrix() const
{
  return _projection;
}

///////////////////////////////////////////////////////////////////////////////

  ///

const Matrix&
Camera::getViewMatrix() const
{
  return _viewMatrix;
}

///////////////////////////////////////////////////////////////////////////////

/// returns position of the camera !!! Works only if position was set via lookAt(Point3 ...)

/*virtual*/
const Point3&
Camera::getPosition()
{
  return _cameraPosition;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
