
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



#ifndef H_GLOOST_RAY
#define H_GLOOST_RAY



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/Point3.h>
#include <gloost/Vector3.h>


namespace gloost
{




  ///  ray with point, vector and length

class Ray
{
	public:


    /// class constructor
    Ray();
    /// class constructor
    Ray(const Ray& ray);
    /// class constructor
    Ray(const Point3& origin, const Vector3& direction);
    /// class constructor
    Ray(const Point3& origin, const Vector3& direction, mathType t);


    /// class destructor
    ~Ray();


    /// Copy
    const Ray& operator= (const Ray&);


    /// set origin
    void setOrigin(const Point3& origin);
    /// get origin
    Point3& getOrigin();
    const Point3& getOrigin() const;


    /// set direction
    void setDirection(const Vector3& direction);
    /// get direction
    Vector3& getDirection();
    const Vector3& getDirection() const;


    /// set t
    void setT(mathType t);
    /// Set t to maximal value
    void setTMax();
    /// get t
    const mathType& getT() const;

    mathType& getT();


    /// get the point origin + direction * t
    Point3 getDestination() const;


    /// normalize length of direction
    void normalize();

    /// returns a normalized version of the ray
    Ray normalized() const;



	private:


    ///
    Point3    _origin;
    Vector3   _direction;
    mathType  _t;



    static mathType T_MAX;


  protected:

    ///


};


extern std::ostream& operator<< (std::ostream&, const Ray&);


} // namespace gloost


#endif // GLOOST_RAY_H

