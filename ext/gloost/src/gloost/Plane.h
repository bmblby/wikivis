
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



#ifndef H_GLOOST_PLANE
#define H_GLOOST_PLANE



// gloost system includes
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/Matrix.h>


// cpp includes
#include <string>



namespace gloost
{


  ///  stores a plane in the HNF notation (which is nice)

class Plane
{
	public:
    // class constructor
    explicit Plane();
    explicit Plane(const Point3& point, const Vector3& normal);
    explicit Plane(const Point3& point0, const Point3& point1, const Point3& point2);

    // class destructor
	  virtual ~Plane();

	  // return true if the Point3 is in front of the Plane
	  virtual bool pointInFrontOf(const Point3& point) const;

	  // returns the Point3 within the plane
	  Point3& getPoint();
	  const Point3& getPoint() const;
	  void setPoint(const Point3& point);

	  // returns the normal of the plane
	  Vector3& getNormal();
	  const Vector3& getNormal() const;
	  void setNormal(const Vector3& point);

	  // transforms the plane by a matrix
	  virtual void transform(const gloost::Matrix& matrix, bool normalizeNormal = true);

	private:

	  Point3  _point;
	  Vector3 _normal;

};


} // namespace gloost


#endif // GLOOST_PLANE_H



