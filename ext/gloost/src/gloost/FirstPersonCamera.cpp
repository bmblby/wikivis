
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



// gloost system includes
#include <gloost/FirstPersonCamera.h>
#include <gloost/Quaternion.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class FirstPersonCamera

  \brief A PerspectiveCamera with an interface supporting first person view

  \author Felix Weiszig
  \date   March 2011
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

FirstPersonCamera::FirstPersonCamera():
    PerspectiveCamera(75.0f, 16.0f/9.0f, 0.01f, 500.0f),
    _position(0.0,0.0,0.0),
    _friction(1.0),
    _rotation(),
    _lookAt(0.0, 0.0,-1.0),
    _upVector(0.0, 1.0, 0.0),
    _dirtyLookAt(true)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
  \remarks ...
*/

FirstPersonCamera::~FirstPersonCamera()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


///**
//  \brief rotates the viewer around itself
//  \param ...
//  \remarks ...
//*/
//
////
//void
//FirstPersonCamera::setRotationH(mathType h)
//{
//
//
//
//
//}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief sets the position of the viewer
  \param ...
  \remarks ...
*/

void
FirstPersonCamera::setPosition(const gloost::Point3 position)
{
  _position = position;
  _dirtyLookAt = true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief moves the viewer
  \param ...
  \remarks ...
*/

void
FirstPersonCamera::move (const Vector3& speed)
{
  Vector3 speedHV(speed[0], speed[1], speed[2]);

  Vector3 directionalSpeed = _viewMatrix.inverted() * speedHV;
  _position += directionalSpeed;
  _position[1] += speed[1];

  _dirtyLookAt = true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief rotate viewer horizontally
  \param ...
  \remarks ...
*/

void
FirstPersonCamera::rotateH (mathType speed)
{

  lookAt(_position,
         _position + _lookAt,
         _upVector);


  _rotation[1] += speed;

  Vector3 curentUp = _viewMatrix.inverted() * Vector3(0.0, 1.0, 0.0);

  Quaternion rotHQuaternion(curentUp, speed);
  _lookAt = rotHQuaternion.rotate(_lookAt);
  _lookAt = Vector3(_lookAt).normalized();

  _upVector = curentUp;

  _dirtyLookAt = true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief rotate viewer vertically
  \param ...
  \remarks ...
*/

void
FirstPersonCamera::rotateV (mathType speed)
{

  lookAt(_position,
         _position + _lookAt,
         _upVector);


  _rotation[0] += speed;

  Vector3 curentAxisX = _viewMatrix.inverted() * Vector3(1.0, 0.0, 0.0);

  Vector3 curentUp    = _viewMatrix.inverted() * Vector3(0.0, 1.0, 0.0);

  Quaternion rotHQuaternion(curentAxisX, speed);
  _lookAt = rotHQuaternion.rotate(_lookAt);
  _lookAt = Vector3(_lookAt).normalized();

  _upVector = curentUp;
  _dirtyLookAt = true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief sets the up vector
  \param ...
  \remarks ...
*/

void
FirstPersonCamera::setUpVector(const gloost::Vector3& upVector)
{
  _upVector    = upVector;
  _dirtyLookAt = true;
}

////////////////////////////////////////////////////////////////////////////////
#if 0

/**
  \brief applies the modelview and projection to the GL state
  \param ...
  \remarks ...
*/

/*virtual*/
void
FirstPersonCamera::set ()
{

  if (_dirtyLookAt)
  {
    lookAt(_position,
           _position + _lookAt,
           _upVector);

    _dirtyLookAt = false;
  }

  PerspectiveCamera::set();
}

#endif
////////////////////////////////////////////////////////////////////////////////


/**
  \brief WORD
  \param ...
  \remarks ...
*/


} // namespace gloost


