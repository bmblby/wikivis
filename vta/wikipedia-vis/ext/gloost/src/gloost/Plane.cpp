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
#include <gloost/Plane.h>
#include <gloost/Vector3.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////

Plane::Plane():
	  _point(0.0,0.0,0.0),
	  _normal(0.0,1.0,0.0)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

Plane::Plane(const Point3& point, const Vector3& normal):
	  _point(point),
	  _normal(normal)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

Plane::Plane(const Point3& point1, const Point3& point2, const Point3& point3):
	  _point(point1),
	  _normal()
{
  Vector3 a(point2 - point1);
  Vector3 b(point3 - point1);
  _normal = cross(a, b).normalized();
}


///////////////////////////////////////////////////////////////////////////////

/*virtual*/
Plane::~Plane()
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////


  /// returns the Point3 within the plane

Point3&
Plane::getPoint()
{
	return _point;
}

///////////////////////////////////////////////////////////////////////////////


  /// returns the Point3 within the plane

const
Point3&
Plane::getPoint() const
{
	return _point;
}

///////////////////////////////////////////////////////////////////////////////

void
Plane::setPoint(const Point3& point)
{
	_point = point;
}

///////////////////////////////////////////////////////////////////////////////


  /// returns the normal of the plane

Vector3&
Plane::getNormal()
{
	return _normal;
}

///////////////////////////////////////////////////////////////////////////////


  /// returns the normal of the plane

const
Vector3&
Plane::getNormal() const
{
	return _normal;
}

///////////////////////////////////////////////////////////////////////////////

void
Plane::setNormal(const Vector3& normal)
{
	_normal = normal;
}

///////////////////////////////////////////////////////////////////////////////


  /// return true if the Point3 is in front of the Plane

bool
Plane::pointInFrontOf(const Point3& point) const
{
	return  ((point - _point)*_normal) > 0.0;
}


///////////////////////////////////////////////////////////////////////////////

  /// transforms the plane by a matrix

void
Plane::transform(const gloost::Matrix& matrix, bool normalizeNormal)
{
  _point  = matrix * _point;
  _normal = matrix * _normal;

	if(normalizeNormal)
  {
    _normal.normalize();
  }
}




///////////////////////////////////////////////////////////////////////////////





} // namespace gloost


