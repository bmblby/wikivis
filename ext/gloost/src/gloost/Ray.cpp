
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
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/Ray.h>



// cpp includes
#include <math.h>
#include <limits>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Ray::Ray():
  _origin(0,0,0),
  _direction(0,0,-1),
  _t(1.0)
{

}


///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Ray::Ray(const Ray& ray):
  _origin(ray._origin),
  _direction(ray._direction),
  _t(ray._t)
{

}


///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Ray::Ray(const Point3& origin, const Vector3& direction):
  _origin(origin),
  _direction(direction),
  _t(1.0)
{

}


///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Ray::Ray(const Point3& origin, const Vector3& direction, mathType t):
  _origin(origin),
  _direction(direction),
  _t(t)
{

}


///////////////////////////////////////////////////////////////////////////////


 /// class destructor

Ray::~Ray()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


 /// copy

const Ray&
Ray::operator= (const Ray& a)
{
  if (this != &a)
  {
    _origin    = a._origin;
    _direction = a._direction;
    _t         = a._t;
  }
  return *this;
}


///////////////////////////////////////////////////////////////////////////////


 ///

///
void
Ray::setOrigin(const Point3& origin)
{
	_origin = origin;
}


///////////////////////////////////////////////////////////////////////////////


 ///

Point3&
Ray::getOrigin()
{
	return _origin;
}


///////////////////////////////////////////////////////////////////////////////


 ///

const Point3&
Ray::getOrigin() const
{
	return _origin;
}


///////////////////////////////////////////////////////////////////////////////


 ///

void
Ray::setDirection(const Vector3& direction)
{
  _direction = direction;
}


///////////////////////////////////////////////////////////////////////////////


 ///

Vector3&
Ray::getDirection()
{
  return _direction;
}


///////////////////////////////////////////////////////////////////////////////


 ///

const Vector3&
Ray::getDirection() const
{
  return _direction;
}


///////////////////////////////////////////////////////////////////////////////


 ///

void
Ray::setT(mathType t)
{
	_t = t;
}


///////////////////////////////////////////////////////////////////////////////


 ///

void
Ray::setTMax()
{
	_t = T_MAX;
}


///////////////////////////////////////////////////////////////////////////////


 ///

const mathType&
Ray::getT() const
{
	return _t;
}



///////////////////////////////////////////////////////////////////////////////


 ///

mathType&
Ray::getT()
{
	return _t;
}


///////////////////////////////////////////////////////////////////////////////


  /// get the point origin*(direction*t)

Point3
Ray::getDestination() const
{
  return _origin + _t*_direction;
}


///////////////////////////////////////////////////////////////////////////////


  /// normalize length of direction

void
Ray::normalize()
{
	_direction.normalize();
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a normalized version of the ray

Ray
Ray::normalized() const
{
	Ray normRay = (*this);
	normRay.normalize();
	return normRay;
}


////////////////////////////////////////////////////////////////////////////////


  ///

/* extern */
std::ostream&
operator<< (std::ostream& os, const Ray& r)
{
  os << std::endl << "ray{";
  os << std::endl <<  "  origin:      " << r.getOrigin();
  os << std::endl <<  "  direction:   " << r.getDirection();
  os << std::endl <<  "  destination: " << r.getDestination();
  os << std::endl <<  "  t:      " << r.getT();
  os << std::endl <<  "}" << std::endl;
  return os;
}


////////////////////////////////////////////////////////////////////////////////



/*static*/ mathType Ray::T_MAX = std::numeric_limits<mathType>::max();



} // namespace gloost
