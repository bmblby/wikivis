
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



#ifndef GLOOST_VECTOR3_H
#define GLOOST_VECTOR3_H



// gloost includes
#include <gloost/gloostConfig.h>



// cpp includes
#include <cmath>
#include <iostream>
#include <fstream>



namespace gloost
{

  class Point3;




 /// 3D vector

class Vector3
{

  public:

    // create default (0.0, 0.0, 0.0) vector
    Vector3();

    // copy constructor
    Vector3(const Vector3&);

    // create a vector from a point (!!! fourth component will be set to 0.0)
    Vector3(const Point3&);

    // initialize with values
    Vector3(const mathType, const mathType, const mathType);

    // for use as quaternions, do not use for 3d coords
    Vector3(const mathType, const mathType, const mathType, const mathType);

    // read three space seperated floats from a ifstream into the vector
//    Vector3(std::ifstream& in);



    // indexing
    mathType& operator[](unsigned);
    const mathType& operator[](unsigned) const;


    // converts to bool: is TRUE if length2 > GLOOST_MATHTYPE_MIN_EPSILON
//    operator bool() const;


    // assignment
    Vector3& operator= (const Vector3&);

    // operator for notation Vector3+=Vector3
    void operator+=(const Vector3&);

    // operator for notation Vector3-=Vector3
    void operator-=(const Vector3&);

    // operator for notation Vector3*=mathType
    void operator*= (const mathType&);

    // operator for notation Vector3/=mathType
    void operator/= (const mathType&);

    // operator for notation Vector3/=Vector3
    void operator/= (const Vector3&);

    // scale this vector (obsolete)
    void scale(const mathType );



    // normalize the vector
    void normalize();

    // returns a normalized copy of this vector
    Vector3 normalized() const;

    // length of the vector
    mathType length() const;

    // quadratic length of the vector
    mathType length2() const;


    // returns the product of all three components
    mathType getComponentProduct() const;

    // returns the sum of all three components
    mathType getComponentSum() const;

    // returns the index of the coponent with the biggest absolute value
    unsigned int getMainAxis() const;

    // returns the biggest component
    const mathType& getMainComponent() const;

//    // returns the smnallest component
//    mathType& getSmallestComponent() const;

    // switches the values of two axis
    void flipAxis(unsigned axis1, unsigned axis2);


    // contrains the vector to a particluar length but preserves the direction
    void constrain (const mathType length);

    //  returns a contrained copy of the vector with a length but the same direction
    Vector3 constrained (const mathType length);



    // returns a copy of this vector with components converted to int
    Vector3 int_copy() const;



    // compresses a normalized normal(12 bytes) to unsigned (4 bytes)
    static unsigned compressAsNormal (const Vector3& vector, unsigned char fourthByte = 0);

    // uncompresses an unsigned (4 bytes) a to normalized normal(12 bytes)
    static Vector3 uncompressAsNormal (unsigned value);

    // compresses a Vector3 with positive values (12 bytes) to unsigned (4 bytes)
    static unsigned compressAsColor (const Vector3& vector, unsigned char fourthByte = 0);

    // uncompresses an unsigned (4 bytes) a to a Vector3 with positive values (12 bytes)
    static Vector3 uncompressAsColor (unsigned value);



    // get pointer to the vectors data as array
    const mathType* data() const;
    mathType* data();

private:

    mathType _data[4];

};



/// vector addition
extern Vector3 operator+(const Vector3&, const Vector3&);

/// vector substraction
extern Vector3 operator-(const Vector3&, const Vector3&);

/// Point3 - Point3
extern Vector3 operator-(const Point3&, const Point3&);

/// dot product of two vectors
extern mathType operator*(const Vector3&, const Vector3&);



/// cross product
extern Vector3 cross(const Vector3&, const Vector3&);

/// component wise multiplication
extern Vector3 mult(const Vector3&, const Vector3&);

/// mixes two Vector3
extern Vector3 mix(const Vector3&, const Vector3&, float value);



/// operator for notation Vector3*mathType
extern Vector3 operator* (const Vector3&, const mathType&);

/// operator for notation mathType*Vector3
extern Vector3 operator* (const mathType&, const Vector3&);


/// operator for notation Vector3/mathType
extern Vector3 operator/ (const Vector3&, const mathType&);

/// operator for notation Vector3/Vector3
extern Vector3 operator/ (const Vector3&, const Vector3&);


/// equal
extern bool operator== (const Vector3&, const Vector3&) ;

/// not equal
extern bool operator!= (const Vector3&, const Vector3&);


/// ostream operator
extern std::ostream& operator<< (std::ostream&, const Vector3&);


} // namespace gloost

#endif // GLOOST_VECTOR3_H
