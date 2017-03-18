
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
#include <gloost/PerspectiveCamera.h>
#include <gloost/gloostConfig.h>
#include <gloost/gl/gloostGlUtil.h>
#include <gloost/gloostMath.h>



// cpp includes



namespace gloost
{


///////////////////////////////////////////////////////////////////////////////


/// class constructor
PerspectiveCamera::PerspectiveCamera():
    Camera()
{
  updateProjection();
}


///////////////////////////////////////////////////////////////////////////////


/// class constructor
PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& perpCamera):
    Camera(perpCamera._fov,
           perpCamera._aspectRatio,
           perpCamera._nearPlaneZ,
           perpCamera._farPlaneZ )
{
  _projection_dirty = perpCamera._projection_dirty;
  updateProjection();
    updateFrustum();

//  this->_projection = perpCamera._projection;
//  this->_modelview  = perpCamera._modelview;
//  this->_frustum    = perpCamera._frustum;
}


///////////////////////////////////////////////////////////////////////////////


  ///

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearPlaneZ, float farPlaneZ)
  : Camera(fov, aspect, nearPlaneZ, farPlaneZ)
{
  updateProjection();
  updateFrustum();
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
PerspectiveCamera::~PerspectiveCamera()
{

}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PerspectiveCamera::setFov(float fov)
{
  _fov = fov;
  _projection_dirty = true;
}


///////////////////////////////////////////////////////////////////////////////


  ///

float
PerspectiveCamera::getFov() const
{
  return _fov;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PerspectiveCamera::setAspectRatio(float aspect)
{
  _aspectRatio = aspect;
  _projection_dirty = true;
  updateProjection();
  updateFrustum();
}


///////////////////////////////////////////////////////////////////////////////


  ///
float
PerspectiveCamera::getAspectRatio() const
{
  return _aspectRatio;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void PerspectiveCamera::setNear(float nearPlaneZ)
{
  _nearPlaneZ = nearPlaneZ;
  _projection_dirty = true;
}


///////////////////////////////////////////////////////////////////////////////


  ///

float
PerspectiveCamera::getNear() const
{
  return _nearPlaneZ;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void PerspectiveCamera::setFar(float farPlaneZ)
{
  _farPlaneZ = farPlaneZ;
  _projection_dirty = true;
}


///////////////////////////////////////////////////////////////////////////////


  ///

float
PerspectiveCamera::getFar() const
{
  return _farPlaneZ;
}


///////////////////////////////////////////////////////////////////////////////

#if 0
  ///

/* virtual */
void PerspectiveCamera::set()
{

  if(_projection_dirty)
  {
    updateProjection();
    updateFrustum();
  }

  Camera::set();
}
#endif

///////////////////////////////////////////////////////////////////////////////


  /// calculates a modelview matrix from given position, point to look at and down vector of the camera

void
PerspectiveCamera::lookAt(const Point3&  cameraPosition,
                          const Point3&  pointToLookAt,
                          const Vector3& upVector)
{

  _cameraPosition = cameraPosition;

#if 0
//  glMatrixMode(GL_MODELVIEW);
//
//  glLoadIdentity();
//
//  glPushMatrix();
//  {
//    glLoadIdentity();
//    gluLookAt((GLfloat) cameraPosition[0],(GLfloat) cameraPosition[1],(GLfloat) cameraPosition[2],
//              (GLfloat) pointToLookAt[0],(GLfloat) pointToLookAt[1],(GLfloat) pointToLookAt[2],
//              (GLfloat) upVector[0], (GLfloat) upVector[1], (GLfloat) upVector[2]);
//    gloostGetv (GL_MODELVIEW_MATRIX, _modelview.data());
//
//
//  }
//  glPopMatrix();
#else

    _viewMatrix.setIdentity();
    gloost::gl::gloostLookAt(_viewMatrix, cameraPosition, pointToLookAt, upVector);

#endif

  updateProjection();
  updateFrustum();

}


///////////////////////////////////////////////////////////////////////////////


  /// returns position of the camera !!! Works only if position was set via lookAt(Point3 ...)

/* virtual */
const Point3&
PerspectiveCamera::getPosition()
{
  return _cameraPosition;
}

///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void PerspectiveCamera::setProjectionMatrix(const Matrix& projectionMatrix)
{
  Camera::setProjectionMatrix(projectionMatrix);
  updateFrustum();
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void PerspectiveCamera::setViewMatrix(const Matrix& viewMatrix)
{
  Camera::setViewMatrix(viewMatrix);
  updateFrustum();
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
Ray
PerspectiveCamera::getPickRay(unsigned int screenWidth,
                              unsigned int screenHeight,
                              unsigned int mouseH,
                              unsigned int mouseV)
{
  if(_projection_dirty)
  {
    updateProjection();
    updateFrustum();
  }

  Vector3 frustumH_vec         = _frustum.far_lower_right - _frustum.far_lower_left;
  Vector3 FrustumOnePixelWidth = frustumH_vec/screenWidth;


  Vector3 frustumV_vec          = _frustum.far_upper_left - _frustum.far_lower_left;
  Vector3 FrustumOnePixelHeight = frustumV_vec/screenHeight;


  Point3 pickPointOnFarPlane = _frustum.far_lower_left
                                + (FrustumOnePixelWidth * mouseH)
                                + (FrustumOnePixelHeight * mouseV);

//  Point3 camPos   = _modelview.inverted() * Point3(0.0, 0.0, 0.0); // _cameraPosition
  Vector3 pickDir = (pickPointOnFarPlane - _cameraPosition).normalized();

  return Ray(_cameraPosition, pickDir);
}


///////////////////////////////////////////////////////////////////////////////


  /// recalc the projection matrix

/*virtual*/
void
PerspectiveCamera::updateProjection()
{
  gl::gloostPerspective(_projection, _fov, _aspectRatio, _nearPlaneZ, _farPlaneZ);
  _projection_dirty = false;

//  static unsigned index = 0;
//  index++;
//  std::cerr << std::endl << "PerspectiveCamera::updateProjection(): " << index;
}


///////////////////////////////////////////////////////////////////////////////


  ///

/*virtual*/
void
PerspectiveCamera::updateFrustum()
{
  const float scale = tan(math::PI * _fov/360.0f);
  const float near_top = _nearPlaneZ * scale;
  const float near_left  = near_top * _aspectRatio;
  const float far_top = _farPlaneZ * scale;
  const float far_left  = far_top * _aspectRatio;
  Matrix view_inv(_viewMatrix);
  view_inv.invert();
  _frustum.near_lower_left  = view_inv * Point3(-near_left,-near_top,-_nearPlaneZ);
  _frustum.near_lower_right = view_inv * Point3(near_left,-near_top,-_nearPlaneZ);
  _frustum.near_upper_right = view_inv * Point3(near_left,near_top,-_nearPlaneZ);
  _frustum.near_upper_left  = view_inv * Point3(-near_left,near_top,-_nearPlaneZ);

  _frustum.far_lower_left  = view_inv * Point3(-far_left,-far_top,-_farPlaneZ);
  _frustum.far_lower_right = view_inv * Point3(far_left,-far_top,-_farPlaneZ);
  _frustum.far_upper_right = view_inv * Point3(far_left,far_top,-_farPlaneZ);
  _frustum.far_upper_left  = view_inv * Point3(-far_left,far_top,-_farPlaneZ);

  _frustum.recalcPlanes();
}


///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
