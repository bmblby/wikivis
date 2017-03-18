
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
#include <gloost/OrientedPlane.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   OrientedPlane

  \brief   A plane consisting of a point, a normal vector and a tangent vector

  \author  Felix Weiszig
  \date    january 2015
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


  /*static*/ const gloost::Vector3 OrientedPlane::referenceTangent = gloost::Vector3(1.0,0.0,0.0);

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

OrientedPlane::OrientedPlane():
  Plane(),
  _rotation((mathType)0.0)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
OrientedPlane::shared_ptr
OrientedPlane::create()
{
	return shared_ptr(new OrientedPlane());
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
OrientedPlane::~OrientedPlane()
{

}

///////////////////////////////////////////////////////////////////////////////

  /// returns the normal of the plane

Vector3
OrientedPlane::getTangent() const
{
//	return _tangent;
//  gloost::Quaternion::rotate(referenceTangent, )
  return Vector3(0.0,0.0,0.0);

}

///////////////////////////////////////////////////////////////////////////////

//  /// returns the normal of the plane
//
//const Vector3&
//OrientedPlane::getTangent() const
//{
//	return _tangent;
//}

///////////////////////////////////////////////////////////////////////////////

//  /// returns the normal of the plane
//
//void
//OrientedPlane::setTangent(const Vector3& tangent)
//{
//	_tangent = tangent;
//}

///////////////////////////////////////////////////////////////////////////////

  /// returns the rotation of the plane around the normal

mathType
OrientedPlane::getRotation() const
{
	return _rotation;
}

///////////////////////////////////////////////////////////////////////////////

  /// sets the rotation of the plane around the normal

void
OrientedPlane::setRotation(const mathType& rotation)
{
	 _rotation = rotation;
}

///////////////////////////////////////////////////////////////////////////////

  /// calculates and returns the normal of the plane

Vector3
OrientedPlane::getBinormal() const
{
	return cross(getNormal(), getTangent() );
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
OrientedPlane::transform(const gloost::Matrix& matrix, bool normalizeNormal)
{
  Plane::transform(matrix, normalizeNormal);
//  _tangent = matrix * _tangent;
//  if (normalizeNormal)
//  {
////    _tangent.normalize();
//  }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost


