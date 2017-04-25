
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
#include <gloost/BoundingBox.h>
#include <gloost/Vector3.h>
#include <gloost/Point3.h>
#include <gloost/Ray.h>


// cpp includes
#include <limits>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(const BoundingBox& aabb):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(aabb._pMin),
  _pMax(aabb._pMax)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(mathType pInitX, mathType pInitY, mathType pInitZ):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(pInitX, pInitY, pInitZ),
  _pMax(pInitX, pInitY, pInitZ)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(mathType pMinX, mathType pMinY, mathType pMinZ,
                         mathType pMaxX, mathType pMaxY, mathType pMaxZ):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(pMinX, pMinY, pMinZ),
  _pMax(pMaxX, pMaxY, pMaxZ)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(const Point3& pInit):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(pInit),
  _pMax(pInit)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(const Point3& pMin, const Point3& pMax):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(pMin),
  _pMax(pMax)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(const std::vector<gloost::Point3>& points):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(0,0,0),
  _pMax(0,0,0)
{
  if (! points.size())
  {
    return;
  }
  _pMin = points[0];
  _pMax = points[0];

  for (unsigned i=0u; i!=points.size(); ++i)
  {
    expand(points[i]);
  }
}

///////////////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox():
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(0.0,0.0,0.0),
  _pMax(0.0,0.0,0.0)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////

 /// class destructor

BoundingBox::~BoundingBox()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

 /// copy

const BoundingBox&
BoundingBox::operator= (const BoundingBox& b)
{
  if (this != &b)
  {
    _pMin = b._pMin;
    _pMax = b._pMax;
  }
  return *this;
}

///////////////////////////////////////////////////////////////////////////////

 /// expand

void
BoundingBox::expand(const Point3& point)
{
//  bool expanded = false;

  for (unsigned i=0; i!=3; ++i)
  {
    if (_pMin[i] > point[i])
    {
      _pMin[i] = point[i];
//      expanded = true;
    }
    if (_pMax[i] < point[i])
    {
      _pMax[i] = point[i];
//      expanded = true;
    }
  }

//  return expanded;
}

///////////////////////////////////////////////////////////////////////////////

 /// expand

void
BoundingBox::expand(const BoundingBox& aabb)
{
  auto result = (*this) + aabb;
  _pMin = result._pMin;
  _pMax = result._pMax;
}

///////////////////////////////////////////////////////////////////////////////

 ///

void
BoundingBox::setPMin(const Point3& pMin)
{
	_pMin = pMin;
}

///////////////////////////////////////////////////////////////////////////////

 ///

const Point3&
BoundingBox::getPMin() const
{
	return _pMin;
}

///////////////////////////////////////////////////////////////////////////////

 ///

Point3&
BoundingBox::getPMin()
{
	return _pMin;
}

///////////////////////////////////////////////////////////////////////////////

 ///

void
BoundingBox::setPMax(const Point3& pMax)
{
	_pMax = pMax;
}

///////////////////////////////////////////////////////////////////////////////

 ///

const Point3&
BoundingBox::getPMax() const
{
	return _pMax;
}

///////////////////////////////////////////////////////////////////////////////

 ///

Point3&
BoundingBox::getPMax()
{
	return _pMax;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the eight points of this aabb
  \remarks ...
*/

std::vector<Point3>
BoundingBox::getPoints() const
{
	std::vector<Point3> points(8u);
	points[0] = Point3(_pMin[0], _pMin[1], _pMin[2]);
	points[1] = Point3(_pMin[0], _pMin[1], _pMax[2]);
	points[2] = Point3(_pMin[0], _pMax[1], _pMin[2]);
	points[3] = Point3(_pMin[0], _pMax[1], _pMax[2]);
	points[4] = Point3(_pMax[0], _pMin[1], _pMin[2]);
	points[5] = Point3(_pMax[0], _pMin[1], _pMax[2]);
	points[6] = Point3(_pMax[0], _pMax[1], _pMin[2]);
	points[7] = Point3(_pMax[0], _pMax[1], _pMax[2]);
	return points;
}

///////////////////////////////////////////////////////////////////////////////

 /// returns the center of the bounding box

Point3
BoundingBox::getCenter() const
{
	return (_pMin + _pMax) * 0.5;
}

///////////////////////////////////////////////////////////////////////////////

 /// returns a vector storing the side length along each axis

Vector3
BoundingBox::getSize() const
{
	return Vector3( std::abs(_pMax[0] - _pMin[0]),
                  std::abs(_pMax[1] - _pMin[1]),
                  std::abs(_pMax[2] - _pMin[2]) );
}

///////////////////////////////////////////////////////////////////////////////

 /// sets the size of the aabb by setting PMax to PMin+size

void
BoundingBox::setSize(const Vector3& size)
{
	_pMax = _pMin;
	expand(_pMin+size);
}

///////////////////////////////////////////////////////////////////////////////

 /// returns a vector storing half the side length along each axis

Vector3
BoundingBox::getHalfSize() const
{
	return Vector3( std::abs(_pMax[0] - _pMin[0]),
                  std::abs(_pMax[1] - _pMin[1]),
                  std::abs(_pMax[2] - _pMin[2])) * 0.5;
}

///////////////////////////////////////////////////////////////////////////////

 /// returns the volume of the boundig box

mathType
BoundingBox::getVolume() const
{
	const Vector3 size = getSize();
	return size[0]*size[1]*size[2];
}


///////////////////////////////////////////////////////////////////////////////

 /// returns true if pMin.i is smaller as pMax.i for i e {x,y,z}

bool
BoundingBox::isValid() const
{
	return (_pMin[0] < _pMax[0] && _pMin[1] < _pMax[1] && _pMin[2] < _pMax[2]);
}

///////////////////////////////////////////////////////////////////////////////

 /// intersect with a ray

bool
BoundingBox::intersect(Ray& ray) const
{

	// source: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm


	/*
	For each pair of planes P associated with X, Y, and Z do:
  (example using X planes)
  if direction Xd = 0 then the ray is parallel to the X planes.
  So, if origin Xo is not between the slabs ( Xo < Xl or Xo > Xh) then return false
  else, if the ray is not parallel to the plane then
  begin
  compute the intersection distance of the planes
	T1 = (Xl - Xo) / Xd
  T2 = (Xh - Xo) / Xd
  If T1 > T2 swap (T1, T2) // since T1 intersection with near plane

  If T1 > Tnear set Tnear =T1 // want largest Tnear
  If T2 < Tfar set Tfar="T2" // want smallest Tfar

  If Tnear > Tfar box is missed so return false
  If Tfar < 0 box is behind ray return false end

  end of for loop
  If Box survived all above tests, return true with
  intersection point Tnear and exit point Tfar.
	*/

	mathType tNear = std::numeric_limits<mathType>::lowest();
	mathType tFar  = std::numeric_limits<mathType>::max();

	// parallel to plane 0..1
	for (int i=0; i!=3; ++i)
	{
    if (ray.getDirection()[i] == 0)
    {
      if (!(_pMin[i] < ray.getOrigin()[i] && _pMax[i] > ray.getOrigin()[i]))
      {
        return false;
      }
    }


    mathType t1 = (_pMin[i] - ray.getOrigin()[i]) / ray.getDirection()[i];
    mathType t2 = (_pMax[i] - ray.getOrigin()[i]) / ray.getDirection()[i];

    // swap
    if (t1 > t2)
    {
      mathType temp = t1;
      t1 = t2;
      t2 = temp;
    }

    if (t1 > tNear)
    {
      tNear = t1;
    }

    if (t2 < tFar)
    {
      tFar = t2;
    }

    if (tNear > tFar)
    {
      return false;
    }

    if (tFar < 0)
    {
      return false;
    }
	}

  ray.setT(tNear);

  return true;
}

///////////////////////////////////////////////////////////////////////////////

 /// returns true if ray intersects with AABB and writes tMax to the second argument

bool
BoundingBox::intersect(const Ray& ray, mathType& tMin, mathType& tMax) const
{

	// source: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm


	/*
	For each pair of planes P associated with X, Y, and Z do:
  (example using X planes)
  if direction Xd = 0 then the ray is parallel to the X planes.
  So, if origin Xo is not between the slabs ( Xo < Xl or Xo > Xh) then return false
  else, if the ray is not parallel to the plane then
  begin
  compute the intersection distance of the planes
	T1 = (Xl - Xo) / Xd
  T2 = (Xh - Xo) / Xd
  If T1 > T2 swap (T1, T2) // since T1 intersection with near plane

  If T1 > Tnear set Tnear =T1 // want largest Tnear
  If T2 < Tfar set Tfar="T2" // want smallest Tfar

  If Tnear > Tfar box is missed so return false
  If Tfar < 0 box is behind ray return false end

  end of for loop
  If Box survived all above tests, return true with
  intersection point Tnear and exit point Tfar.
	*/

	mathType tNear = std::numeric_limits<mathType>::lowest();
	mathType tFar  = std::numeric_limits<mathType>::max();


	// parallel to plane 0..1
	for (int i=0; i!=3; ++i)
	{
    if (ray.getDirection()[i] == 0)
    {
      if (!(_pMin[i] < ray.getOrigin()[i] && _pMax[i] > ray.getOrigin()[i]))
      {
        return false;
      }
    }


    mathType t1 = (_pMin[i] - ray.getOrigin()[i]) / ray.getDirection()[i];
    mathType t2 = (_pMax[i] - ray.getOrigin()[i]) / ray.getDirection()[i];

    // swap
    if (t1 > t2)
    {
      mathType temp = t1;
      t1 = t2;
      t2 = temp;
    }

    if (t1 > tNear)
    {
      tNear = t1;
    }

    if (t2 < tFar)
    {
      tFar = t2;
    }

    if (tNear > tFar)
    {
      return false;
    }

    if (tFar < 0)
    {
      return false;
    }
	}

  tMin = tNear;
  tMax = tFar;

  return true;
}

///////////////////////////////////////////////////////////////////////////////

 /// intersect with a ray

bool
BoundingBox::intersect(Ray& ray, Vector3& normal) const
{

	if (intersect(ray))
	{
    Point3 hitPoint = ray.getDestination();

    if (std::abs(_pMin[0] - hitPoint[0]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(-1.0, 0.0, 0.0);
    }
    else if ( std::abs(_pMax[0] - hitPoint[0]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(1.0, 0.0, 0.0);
    }
    else if (std::abs(_pMin[1] - hitPoint[1]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, -1.0, 0.0);
    }
    else if ( std::abs(_pMax[1] - hitPoint[1]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, 1.0, 0.0);
    }
    else if (std::abs(_pMin[2] - hitPoint[2]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, 0.0, -1.0);
    }
    else if ( std::abs(_pMax[2] - hitPoint[2]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, 0.0, 1.0);
    }

    return true;
	}

  return false;
}

///////////////////////////////////////////////////////////////////////////////

 /// returns the intersection of a AABB with this AABB

BoundingBox
BoundingBox::intersect(const BoundingBox& aabb) const
{
  BoundingBox result = aabb;
  for (unsigned i=0; i!=3; ++i)
  {
    if (_pMin[i] > aabb._pMin[i])
    {
      result._pMin[i] = _pMin[i];
    }
    if (_pMax[i] < aabb._pMax[i])
    {
      result._pMax[i] = _pMax[i];
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////

    /// return TRUE if BoundingBox intersects with frustum TODO: move this function to gloost::Frustum

/*virtual*/ bool
BoundingBox::intersectWithFrustum(const Frustum& frustum) const
{

/*
      _pMin
          g-----h
         /     /|
        /     / |
        c----d  f
  e --> |    | /
        |    |/
        a----b
             _pMax
*/

  gloost::Point3 a(_pMin[0], _pMax[1], _pMax[2]);
  gloost::Point3 b(_pMax);
  gloost::Point3 c(_pMin[0], _pMin[1], _pMax[2]);
  gloost::Point3 d(_pMax[0], _pMin[1], _pMax[2]);
  gloost::Point3 e(_pMin[0], _pMax[1], _pMin[2]);
  gloost::Point3 f(_pMax[0], _pMax[1], _pMin[2]);
  gloost::Point3 g(_pMin);
  gloost::Point3 h(_pMax[0], _pMin[1], _pMin[2]);

  if (frustum.inside(getCenter())) return true;
  if (frustum.inside(a)) return true;
  if (frustum.inside(b)) return true;
  if (frustum.inside(c)) return true;
  if (frustum.inside(d)) return true;
  if (frustum.inside(e)) return true;
  if (frustum.inside(f)) return true;
  if (frustum.inside(g)) return true;
  if (frustum.inside(h)) return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////

/// return true if point p is inside;

/*virtual*/
bool
BoundingBox::contains(const Point3& p) const
{
  return (_pMin[0] <= p[0]) &&
    (_pMin[1] <= p[1]) &&
    (_pMin[2] <= p[2]) &&
    (_pMax[0] >= p[0]) &&
    (_pMax[1] >= p[1]) &&
    (_pMax[2] >= p[2]);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   return TRUE if aabb is within this aabb
  \remarks ...
*/

/*virtual*/
bool
BoundingBox::contains(const BoundingBox& aabb) const
{
  if ( contains(aabb.getPMin()) && contains(aabb.getPMax()) )
  {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

 /// returns TRUE if AABB intersects plane p

bool
BoundingBox::intersectPlane(const Plane& plane) const
{

  int q;
  Vector3 vmin,vmax;
  mathType v;

  Vector3 nPlaneNormal = plane.getNormal().normalized();
  Vector3 halfSides = getHalfSize();

  for(q=0; q<=2; q++)
  {
    v=plane.getPoint()[q];
    if(nPlaneNormal[q]>0.0f)
    {
      vmin[q]=-halfSides[q] - v;
      vmax[q]= halfSides[q] - v;
    }
    else
    {
      vmin[q]= halfSides[q] - v;
      vmax[q]=-halfSides[q] - v;
    }
  }
  if(nPlaneNormal * vmin >  0.0f) return 0;
  if(nPlaneNormal * vmax >= 0.0f) return 1;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns true if aabb fits into this one (without taking positions into account)
*/

bool
BoundingBox::fits(const BoundingBox& aabb) const
{
  const auto mySize   = getSize();
  const auto aabbSize = aabb.getSize();

  for (unsigned i=0u; i!=3u; ++i)
  {
    if (mySize[i] < aabbSize[i])
    {
      return false;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

  /// tarnsform the bounding box with a matrix

/*virtual*/
void
BoundingBox::transform(const Matrix& m)
{
  (*this) = m * (*this);
}

////////////////////////////////////////////////////////////////////////////////

  /// returns a tarnsformed version of this aabb

/*virtual*/
BoundingBox
BoundingBox::transformed(const gloost::Matrix& transformation) const
{
  return transformation * (*this);
}


////////////////////////////////////////////////////////////////////////////////

  ///  returns a matrix to transform a point in  {-0.5,-0.5,-0.5}{0.5,0.5,0.5} to the AABBs local coordinate system

/*virtual*/
gloost::Matrix
BoundingBox::getToLocalMatrix() const
{
  const auto size = getSize();
  const auto mainAxis = size.getMainAxis();

  if (std::abs(size[mainAxis]) < GLOOST_MATHTYPE_MIN_EPSILON)
  {
    return Matrix::createTransMatrix(getCenter());
  }

  return Matrix::createTransMatrix(getCenter()) * Matrix::createScaleMatrix(size[mainAxis]);
}

////////////////////////////////////////////////////////////////////////////////

  /// returns a matrix to transform the local coordinate system of the aabb to a {-0.5,-0.5,-0.5}{0.5,0.5,0.5}

/*virtual*/
gloost::Matrix
BoundingBox::getNormalizeMatrix() const
{
  const auto size = getSize();
  const auto mainAxis = size.getMainAxis();
  if (std::abs(size[mainAxis]) < GLOOST_MATHTYPE_MIN_EPSILON)
  {
    return Matrix::createTransMatrix(getCenter()*-1.0);
  }
  return Matrix::createScaleMatrix(1.0/size[mainAxis]) * Matrix::createTransMatrix(getCenter()*-1.0);
}

////////////////////////////////////////////////////////////////////////////////

  /// operator for notation Matrix*BoundingBox

/*extern*/
BoundingBox
operator* (const Matrix& matrix, const BoundingBox& boundingBox)
{
  return BoundingBox(matrix*boundingBox.getPMin(),
                     matrix*boundingBox.getPMax());
}

////////////////////////////////////////////////////////////////////////////////

  /// adds two bounding boxes so that the result will include both boxes

/*extern*/
BoundingBox
operator+ (const BoundingBox& a, const BoundingBox& b)
{
  const Point3& minA = a.getPMin();
  const Point3& maxA = a.getPMax();

  const Point3& minB = b.getPMin();
  const Point3& maxB = b.getPMax();

  BoundingBox r(a);

  for (unsigned int i=0; i!=3; ++i)
  {
    if (minA[i] > minB[i])
    {
      r.getPMin()[i] = minB[i];
    }

    if (maxA[i] < maxB[i])
    {
      r.getPMax()[i] = maxB[i];
    }
  }

  return r;
}

////////////////////////////////////////////////////////////////////////////////

  /// adds two bounding boxes so that the result will include both boxes

/*extern*/
//BoundingBox
//operator+ (const BoundingBox& bbox, const Point3& b)
//{
//  for (unsigned i=0; i!=3; ++i)
//  {
//    if (bbox)
//  }
//
//  return r;
//}

////////////////////////////////////////////////////////////////////////////////

  ///

/* extern */
std::ostream&
operator<< (std::ostream& os, const BoundingBox& bb)
{
  os << std::endl << "BoundingBox: ("<< std::endl << std::fixed << bb.getPMin() << ", "<< std::endl << bb.getPMax() <<")";
  return os;
}

////////////////////////////////////////////////////////////////////////////////

  /// equal

/*extern*/
bool operator== (const BoundingBox& a, const BoundingBox& b)
{
  return (a.getPMin() == b.getPMin() && a.getPMax() == b.getPMax());
}

////////////////////////////////////////////////////////////////////////////////

  /// not equal

/*extern*/
bool operator!= (const BoundingBox& a, const BoundingBox& b)
{
  return (a.getPMin() != b.getPMin() || a.getPMax() != b.getPMax());
}

///////////////////////////////////////////////////////////////////////////////

 ///


} // namespace gloost

