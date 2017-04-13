
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



#ifndef GLOOST_BOUNDINGBOX_H
#define GLOOST_BOUNDINGBOX_H



// gloost system includes
#include <gloost/BoundingVolume.h>
#include <gloost/Vector3.h>
#include <gloost/Matrix.h>
#include <gloost/Point3.h>
#include <gloost/Ray.h>


// cpp includes


namespace gloost
{


struct plane
{
  // normal
  Vector3 n;
  // a point on the plane
  Point3  p;
  // a point on the plane
  mathType d;
};


/*
 _pMin
      o-----
     /     /|
    /     / |
    ------  |
    |    | /
    |    |/
    -----o
         _pMax
*/


  ///  Axis aligned bounding box (AABB)

class BoundingBox : public BoundingVolume
{
	public:

    BoundingBox();
    BoundingBox(const BoundingBox& aabb);
    BoundingBox(mathType pInitX, mathType pInitY, mathType pInitZ);
    BoundingBox(mathType pMinX, mathType pMinY, mathType pMinZ,
                mathType pMaxX, mathType pMaxY, mathType pMaxZ);
    BoundingBox(const Point3& pInit);
    BoundingBox(const Point3& pMin, const Point3& pMax);
    BoundingBox(const std::vector<gloost::Point3>& points);

    virtual ~BoundingBox();

    /// assignment
    const BoundingBox& operator= (const BoundingBox& aabb);


    // expands the bbox with a point,
    void expand(const Point3& point);

    // expands the BoundingBox with another BoundingBox,
    void expand(const BoundingBox& aabb);


    ///
    void setPMin(const Point3& pMin);
    ///
    const Point3& getPMin() const;
    Point3& getPMin();
    ///
    void setPMax(const Point3& pMax);
    ///
    const Point3& getPMax() const;
    Point3& getPMax();

    // returns the eight points of this aabb
    std::vector<gloost::Point3> getPoints() const;

    /// returns the center of the bounding box
    Point3 getCenter() const;


    /// returns a vector storing the side length along each axis
    Vector3 getSize() const;

    /// sets the size of the aabb by setting PMax to PMin+size
    void setSize(const Vector3& size);

    /// returns a vector storing half the side length along each axis
    Vector3 getHalfSize() const;

    /// returns the volume of the boundig box
    mathType getVolume() const;


    // returns true if pMin.i is smaller as pMax.i for i e {x,y,z}
    bool isValid() const;


	  /// returns true if ray intersects with AABB
    bool intersect(Ray& ray) const;

	  /// returns true if ray intersects with AABB and writes tMax to the second argument
    bool intersect(const Ray& ray, mathType& tMin, mathType& tMax) const;

	  /// returns true if ray intersects with AABB
    bool intersect(Ray& ray, Vector3& normal) const;

    // returns the intersection of a AABB with this AABB
    BoundingBox intersect(const BoundingBox& aabb) const;

    /// return TRUE if BoundingBox intersects with frustum
    /*virtual*/ bool intersectWithFrustum(const Frustum& frustum) const;

    /// return TRUE if point p is within this aabb;
    /*virtual*/ bool contains(const Point3& p) const;

    /// return TRUE if aabb is within this aabb;
    /*virtual*/ bool contains(const BoundingBox& aabb) const;

    // returns TRUE if AABB intersects plane p
    bool intersectPlane(const Plane& plane) const;

    // returns true if aabb fits into this one (without taking positions into account)
    bool fits(const BoundingBox& aabb) const;

    /// tarnsform the bounding box with a matrix
    /*virtual*/ void transform(const Matrix& m);

    /// returns a tarnsformed version of this aabb
    /*virtual*/BoundingBox transformed(const gloost::Matrix& transformation) const;

    /// returns a matrix to transform a point in  {-0.5,-0.5,-0.5}{0.5,0.5,0.5} to the AABBs local coordinate system
    gloost::Matrix getToLocalMatrix() const;

    /// returns a matrix to transform the local coordinate system of the aabb to a
    gloost::Matrix getNormalizeMatrix() const;


  protected:

    /// min and max point;
    Point3 _pMin;
    Point3 _pMax;
};

typedef BoundingBox Aabb;

/// operator for notation Matrix*BoundingBox
extern BoundingBox operator* (const Matrix&, const BoundingBox&);

/// adds two bounding boxes so that the result will include both boxes
extern BoundingBox operator+(const BoundingBox&, const BoundingBox&);
extern BoundingBox operator+(const BoundingBox&, const Point3&);

/// equal
extern bool operator== (const BoundingBox&, const BoundingBox&);

/// not equal
extern bool operator!= (const BoundingBox&, const BoundingBox&);


/// ostream operator
extern std::ostream& operator<< (std::ostream&, const BoundingBox&);



} // namespace gloost


#endif // GLOOST_BOUNDINGBOX_H
