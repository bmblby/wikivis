
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
#include <gloost/ScreenCamera.h>
#include <gloost/Point3.h>
#include <gloost/gl/gloostGlUtil.h>
#include <gloost/gloostMath.h>


// cpp includes


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////

///

ScreenCamera::ScreenCamera(const Matrix& cyclopsmatrix,
                           float nearPlaneZ,
                           float farPlaneZ,
                           float screenwidth,
                           float screenheight)
    : Camera(),
    _cyclops(cyclopsmatrix),
    _eye(),
    _nearPlaneZ(nearPlaneZ),
    _farPlaneZ(farPlaneZ),
    _fov(),
    _screen(),
    _screenw(screenwidth),
    _screenh(screenheight),
      _dirty(true),
      _left(),
      _right(),
      _bottom(),
      _top()
{
  _eye.setIdentity();
  _screen.setIdentity();
}


///////////////////////////////////////////////////////////////////////////////


///

ScreenCamera::ScreenCamera(const Matrix& cyclopsmatrix,
                           float nearPlaneZ,
                           float farPlaneZ,
                           const Matrix& screenmatrix,
                           float screenwidth,
                           float screenheight)
  : Camera(),
    _cyclops(cyclopsmatrix),
    _eye(),
    _nearPlaneZ(nearPlaneZ),
    _farPlaneZ(farPlaneZ),
    _fov(),
    _screen(screenmatrix),
    _screenw(screenwidth),
    _screenh(screenheight),
    _dirty(true),
      _left(),
      _right(),
      _bottom(),
      _top()

{
  _eye.setIdentity();

}


///////////////////////////////////////////////////////////////////////////////


  ///

ScreenCamera::~ScreenCamera()
{

}

///////////////////////////////////////////////////////////////////////////////


///


void ScreenCamera::setCyclopsMatrix(const Matrix& cyclopsmatrix)
{
  _cyclops = cyclopsmatrix;
  _dirty = true;

}



///////////////////////////////////////////////////////////////////////////////


///

void ScreenCamera::setScreenMatrix(const Matrix& screenmatrix)
{
  _screen = screenmatrix;
  _dirty = true;
}


///////////////////////////////////////////////////////////////////////////////


///
void ScreenCamera::setScreenWidth(float screenwidth)
{
  _screenw = screenwidth;
  _dirty = true;
}


///////////////////////////////////////////////////////////////////////////////


///

void ScreenCamera::setScreenHeight(float screenheight)
{
  _screenh = screenheight;
  _dirty = true;
}



///////////////////////////////////////////////////////////////////////////////


  ///

void ScreenCamera::setNear(float nearPlanZ)
{
  _nearPlaneZ = nearPlanZ;
  _dirty = true;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void ScreenCamera::setFar(float farPlanZ)
{
  _farPlaneZ = farPlanZ;
  _dirty = true;
}

///////////////////////////////////////////////////////////////////////////////


  /// set poisition, point to look at and down vector of the camera

void
ScreenCamera::lookAt(Point3 eye, Point3 coi, Vector3 up)
{
  _screen.setIdentity();

  Vector3 sz = eye - coi;
  sz.normalize();
  Vector3 sx = cross(up, sz);
  Vector3 sy = cross(sz, sx);

  _screen[0] = sx[0];
  _screen[1] = sx[1];
  _screen[2] = sx[2];

  _screen[4] = sy[0];
  _screen[5] = sy[1];
  _screen[6] = sy[2];

  _screen[8] = sz[0];
  _screen[9] = sz[1];
  _screen[10] = sz[2];

  _screen[12] = coi[0];
  _screen[13] = coi[1];
  _screen[14] = coi[2];

  _cyclops.setIdentity();
  _cyclops.setTranslate(eye);
  _cameraPosition = eye;
  _dirty = true;


}


///////////////////////////////////////////////////////////////////////////////


  ///

#if 0
/* virtual */
void ScreenCamera::set()
{


  if(_dirty)
  {
    update();
    updateFrustum();

  }

  Camera::set();

}
#endif

float
ScreenCamera::getLeft(){
  return _left;
}

float
ScreenCamera::getRight(){
  return _right;
}

float
ScreenCamera::getBottom(){
  return _bottom;
}

float
ScreenCamera::getTop(){
  return _top;
}



///////////////////////////////////////////////////////////////////////////////


///


/// recalc the projection and modelview matrix
void
ScreenCamera::update()
{

  Point3 eye_World(_eye*Point3(0.0,0.0,0.0));
  eye_World = _cyclops * eye_World;

  Matrix screen_Inv(_screen);
  screen_Inv.invert();



  Point3 eye_Screen = screen_Inv * eye_World;
  Matrix eye_Offset;
  eye_Offset.setIdentity();
  eye_Offset.setTranslate(eye_Screen);
  eye_Offset.invert();


  _viewMatrix = eye_Offset * screen_Inv;

  // distance eye to screen



  float d  =  eye_Screen[2];

  float ox = -eye_Screen[0];
  float oy = -eye_Screen[1];



  _left    = (ox - _screenw/2.0) * _nearPlaneZ/d;
  _right   = (ox + _screenw/2.0) * _nearPlaneZ/d;
  _bottom  = (oy - _screenh/2.0) * _nearPlaneZ/d;
  _top     = (oy + _screenh/2.0) * _nearPlaneZ/d;


  // steppo: dont know if this is correct...feel better now
  _fov = 2.0*atan(_screenw/d);

//  glMatrixMode(GL_PROJECTION);
//  glPushMatrix();
//  {
//    glLoadIdentity();
//    glFrustum( _left, _right, _bottom, _top, _nearPlaneZ, _farPlaneZ);
//    gloostGetv (GL_PROJECTION_MATRIX, _projection.data());
//  }
//  glPopMatrix();

  gl::gloostFrustum(_projection,  _left, _right, _bottom, _top, _nearPlaneZ, _farPlaneZ);



//  glMatrixMode(GL_MODELVIEW);

}


///////////////////////////////////////////////////////////////////////////////


///


void ScreenCamera::updateFrustum()
{


  const float tan_term = tan(math::PI * _fov/180.0f);

  const float inv_aspect = _screenh/_screenw;

  const float near_left = _nearPlaneZ * tan_term;
  const float near_top  = near_left * inv_aspect;
  const float far_left = _farPlaneZ * tan_term;
  const float far_top  = far_left * inv_aspect;


  Matrix view_inv(_viewMatrix);
  view_inv.invert();
  _frustum.near_lower_left = view_inv * Point3(-near_left,-near_top,-_nearPlaneZ);
  _frustum.near_lower_right = view_inv * Point3(near_left,-near_top,-_nearPlaneZ);
  _frustum.near_upper_right = view_inv * Point3(near_left,near_top,-_nearPlaneZ);
  _frustum.near_upper_left = view_inv * Point3(-near_left,near_top,-_nearPlaneZ);

  _frustum.far_lower_left = view_inv * Point3(-far_left,-far_top,-_farPlaneZ);
  _frustum.far_lower_right = view_inv * Point3(far_left,-far_top,-_farPlaneZ);
  _frustum.far_upper_right = view_inv * Point3(far_left,far_top,-_farPlaneZ);
  _frustum.far_upper_left = view_inv * Point3(-far_left,far_top,-_farPlaneZ);



}

///////////////////////////////////////////////////////////////////////////////


/// WIE SIEHT DAS DEN AUS STEPPO??????
/*virtual*/
const Point3&
ScreenCamera::getPosition(){
  if(_dirty)
    {
update();
updateFrustum();

    }
  return _cameraPosition;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
