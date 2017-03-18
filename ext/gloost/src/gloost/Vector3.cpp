
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
#include <gloost/Vector3.h>
#include <gloost/Point3.h>
#include <gloost/gloostMath.h>



// cpp includes
#include <cstring>



namespace gloost
{

////////////////////////////////////////////////////////////////////////////////


Vector3::Vector3():
  _data()
{
//    _data[0]=0.0;
//    _data[1]=0.0;
//    _data[2]=0.0;
//    _data[3]=0.0;
}


////////////////////////////////////////////////////////////////////////////////


Vector3::Vector3 (const Vector3& a):
  _data ()
{
  memcpy(_data, a._data, sizeof(mathType)*4);
}


////////////////////////////////////////////////////////////////////////////////


Vector3::Vector3 (const Point3& p):
  _data ()
{
  memcpy(_data, p.data(), sizeof(mathType)*3);
  _data[3] = 0.0;
}


////////////////////////////////////////////////////////////////////////////////


Vector3::Vector3(const mathType  a,const mathType b,const mathType c)
    : _data()
{
    _data[0]=a;
    _data[1]=b;
    _data[2]=c;
    _data[3]=0.0;
}

////////////////////////////////////////////////////////////////////////////////


  /// for use as quaternions, do not use for 3d coords

Vector3::Vector3(const mathType  a,const mathType b,const mathType c, const mathType w)
    : _data()
{
    _data[0]=a;
    _data[1]=b;
    _data[2]=c;
    _data[3]=w;
}

////////////////////////////////////////////////////////////////////////////////


//Vector3::Vector3(std::ifstream& in)
//    : _data()
//{
//  in >> _data[0];
//  in >> _data[1];
//  in >> _data[2];
//  _data[3]=0.0;
//}


////////////////////////////////////////////////////////////////////////////////


mathType
Vector3::length() const

{
  return (sqrt ((_data[0]*_data[0])+
                (_data[1]*_data[1])+
                (_data[2]*_data[2])));
}


////////////////////////////////////////////////////////////////////////////////


mathType
Vector3::length2() const
{
  return ((_data[0]*_data[0])+
          (_data[1]*_data[1])+
          (_data[2]*_data[2]));
}


////////////////////////////////////////////////////////////////////////////////

  // returns the product of all three components

mathType
Vector3::getComponentProduct() const
{
  return _data[0]*_data[1]*_data[2];
}


////////////////////////////////////////////////////////////////////////////////

  // returns the sum of all three components

mathType
Vector3::getComponentSum() const
{
  return _data[0]+_data[1]+_data[2];
}

///////////////////////////////////////////////////////////////////////////////


  /// contrains the vector to a particluar length but preserves the direction

void
Vector3::constrain (const mathType length)
{
  const mathType l(length2());

  if(l > length*length)
  {
    const mathType factor(length/sqrt(l));

    _data[0]=_data[0]*factor;
    _data[1]=_data[1]*factor;
    _data[2]=_data[2]*factor;
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a contrained copy of the vector with a length but the same direction

Vector3
Vector3::constrained (const mathType length)
{
  Vector3 constrainedVector(*this);
  constrainedVector.constrain(length);
  return constrainedVector;
}


////////////////////////////////////////////////////////////////////////////////

  /// returns the index of the coponent with the biggest value

unsigned int
Vector3::getMainAxis() const
{
  unsigned int index = 0u;
  if ( std::abs(_data[1]) > std::abs(_data[0]) )
  {
    index = 1u;
  }
  if ( std::abs(_data[2]) > std::abs(_data[index]) )
  {
    return 2u;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the biggest component
  \remarks ...
*/

const mathType&
Vector3::getMainComponent() const
{
  return _data[getMainAxis()];
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the smnallest component
  \remarks ...
*/

//mathType
//Vector3::getSmallestComponent() const
//{
//  return math::min(_data[0],_data[1],_data[2]);
//}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   switches the values of two axis
  \remarks ...
*/

void
Vector3::flipAxis(unsigned axis1, unsigned axis2)
{
  const auto tmp = _data[axis1];
  _data[axis1] = _data[axis2];
  _data[axis2] = tmp;
}

////////////////////////////////////////////////////////////////////////////////

  /// returns a copy of this vector with components converted to int

Vector3
Vector3::int_copy() const
{
  return Vector3(int(_data[0]), int(_data[1]), int(_data[2]));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compresses a normalalized normal(12 bytes) to unsigned (4 bytes)
  \param   normal normalized Vector3
  \remarks be sure the normal was normalized befor given to this function
*/

/*static*/
unsigned
Vector3::compressAsNormal(const Vector3& vector, unsigned char fourthByte)
{
  unsigned char nx8 = (unsigned char)((vector[0]*0.5f + 0.5f)*255);
  unsigned char ny8 = (unsigned char)((vector[1]*0.5f + 0.5f)*255);
  unsigned char nz8 = (unsigned char)((vector[2]*0.5f + 0.5f)*255);

  unsigned packedNormal;
  gloost::helper::packRgbaToUnsigned(packedNormal, nx8, ny8, nz8, fourthByte);
  return packedNormal;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   uncompresses an unsigned (4 bytes) a to normalized normal(12 bytes)
  \param
  \remarks
*/

/*static*/
Vector3
Vector3::uncompressAsNormal(unsigned value)
{
  unsigned char nx, ny, nz, empty;
  Vector3 v3;

  gloost::helper::unpackRgbaFromUnsigned(value, nx, ny, nz, empty);
  v3[0] = ((nx*0.003921569)-0.5) * 2.0; // <-- nx
  v3[1] = ((ny*0.003921569)-0.5) * 2.0; // <-- ny
  v3[2] = ((nz*0.003921569)-0.5) * 2.0; // <-- nz

  return v3;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compresses a Vector3 with positive values (12 bytes) to unsigned (4 bytes)
  \param   normal normalized Vector3 with positive values only
  \remarks ...
*/

/*static*/
unsigned
Vector3::compressAsColor(const Vector3& vector, unsigned char fourthByte)
{
  unsigned char red8   = (unsigned char)(math::clamp((float)vector[0], 0.0f, 1.0f)*255);
  unsigned char green8 = (unsigned char)(math::clamp((float)vector[1], 0.0f, 1.0f)*255);
  unsigned char blue8  = (unsigned char)(math::clamp((float)vector[2], 0.0f, 1.0f)*255);

  unsigned packedColor;
  gloost::helper::packRgbaToUnsigned(packedColor, red8, green8, blue8, fourthByte);

  return packedColor;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   uncompresses an unsigned (4 bytes) a to a Vector3 with positive values (12 bytes)
  \param
  \remarks ...
*/

/*static*/
Vector3
Vector3::uncompressAsColor(unsigned value)
{
  unsigned char r,g,b,empty;
  Vector3 v3;
  gloost::helper::unpackRgbaFromUnsigned(value, r, g, b, empty);
  v3[0] = (float)r*0.003921569f; // <-- red
  v3[1] = (float)g*0.003921569f; // <-- green
  v3[2] = (float)b*0.003921569f; // <-- blue
  return v3;
}

////////////////////////////////////////////////////////////////////////////////

 /// get pointer to an array

const mathType*
Vector3::data() const
{
  return _data;
}

////////////////////////////////////////////////////////////////////////////////

 /// get pointer to an array

mathType*
Vector3::data()
{
  return _data;
}

////////////////////////////////////////////////////////////////////////////////

mathType&
Vector3::operator[](unsigned a)
{
    return (4 < a) ? _data[0] : _data[a];
}

////////////////////////////////////////////////////////////////////////////////

const mathType&
Vector3::operator[] (unsigned a) const
{
  return (4 < a) ? _data[0] : _data[a];
}

////////////////////////////////////////////////////////////////////////////////

Vector3&
Vector3::operator= (const Vector3& v)
{
  memcpy(_data, v._data, sizeof(mathType)*4);
  return (*this);
}

////////////////////////////////////////////////////////////////////////////////

/**
    \brief converts to bool: is TRUE if length2 > GLOOST_MATHTYPE_MIN_EPSILON

*/
//Vector3::operator bool() const
//{
//  return length2() > GLOOST_MATHTYPE_MIN_EPSILON;
//}

////////////////////////////////////////////////////////////////////////////////

  /// scale this vector

void
Vector3::scale (const mathType r)
{
  _data[0] *= r;
  _data[1] *= r;
  _data[2] *= r;

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_DEVNOTES
  std::cout << std::endl;
  std::cout << std::endl << "Warning from Vector3::scale (const mathType r)" ;
  std::cout << std::endl << "             This method is obsolete ...";
  std::cout << std::endl << "             Use the * operator instead.";
  std::flush(std::cout);
#endif

}

////////////////////////////////////////////////////////////////////////////////

  /// normalize the vector

void
Vector3::normalize()
{
	const mathType l(length());

  if( std::abs(l)>GLOOST_MATHTYPE_MIN_EPSILON )
  {
	  _data[0]=_data[0]/l;
	  _data[1]=_data[1]/l;
	  _data[2]=_data[2]/l;
  }
}

////////////////////////////////////////////////////////////////////////////////

  /// returns a normalized copy of this vector

Vector3
Vector3::normalized() const
{
	Vector3 nn(*this);
	nn.normalize();

  return nn;
}

////////////////////////////////////////////////////////////////////////////////

/// vector addition

/*extern*/
Vector3 operator+(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0] + b[0],
                 a[1] + b[1],
                 a[2] + b[2]);
}

////////////////////////////////////////////////////////////////////////////////

/// vector addition

void
Vector3::operator+=(const Vector3& b)
{
  _data[0] += b[0];
  _data[1] += b[1];
  _data[2] += b[2];
}

////////////////////////////////////////////////////////////////////////////////

/// vector substraction

/*extern*/
Vector3 operator-(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0] - b[0],
                 a[1] - b[1],
                 a[2] - b[2]);
}

////////////////////////////////////////////////////////////////////////////////

/// vector substraction

void
Vector3::operator-=(const Vector3& b)
{
  _data[0] -= b[0];
  _data[1] -= b[1];
  _data[2] -= b[2];
}

////////////////////////////////////////////////////////////////////////////////

/// /// Point3 - Point3

/*extern*/
Vector3 operator-(const Point3& a, const Point3& b)
{
  return Vector3(a[0] - b[0],
                 a[1] - b[1],
                 a[2] - b[2]);
}

////////////////////////////////////////////////////////////////////////////////

/// dot product of two vectors

/*extern*/
mathType operator*(const Vector3& a, const Vector3& b)
{
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

////////////////////////////////////////////////////////////////////////////////

/// cross product

/*extern*/
Vector3 cross(const Vector3& a, const Vector3& b)
{
  return Vector3(((a[1]*b[2]) - (a[2]*b[1])),
	               ((a[2]*b[0]) - (a[0]*b[2])),
                 ((a[0]*b[1]) - (a[1]*b[0]))) ;
}

////////////////////////////////////////////////////////////////////////////////

/// extern Vector3 mult(const Vector3&, const Vector3&);

/*extern*/
Vector3 mult(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0]*b[0], a[1]*b[1], a[2]*b[2]);
}

////////////////////////////////////////////////////////////////////////////////

/// mixes two Vector3s

/*extern*/
Vector3 mix(const Vector3& a, const Vector3& b, float value)
{
  float oneMinusValue = 1.0 - value;

  return Vector3(a[0]*oneMinusValue+b[0]*value,
                 a[1]*oneMinusValue+b[1]*value,
                 a[2]*oneMinusValue+b[2]*value);
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation Vector3*mathType

/*extern*/
Vector3 operator* (const Vector3& a, const mathType& s)
{
  return Vector3(a[0]*s, a[1]*s, a[2]*s);
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation Vector3*=mathType

void
Vector3::operator*= (const mathType& s)
{
  _data[0] *= s;
  _data[1] *= s;
  _data[2] *= s;
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation mathType*Vector3

/*extern*/
Vector3 operator* (const mathType& s, const Vector3& a)
{
  return Vector3(a[0]*s, a[1]*s, a[2]*s);
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation Vector3/mathType

/*extern*/
Vector3 operator/ (const Vector3& a, const mathType& s)
{
  mathType d = 1.0/s;
  return Vector3(a[0]*d, a[1]*d, a[2]*d);
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation Vector3/=mathType

void
Vector3::operator/= (const mathType& s)
{
  const mathType d = 1.0/s;
  _data[0] *= d;
  _data[1] *= d;
  _data[2] *= d;
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation Vector3/Vector3

/*extern*/
Vector3 operator/ (const Vector3& a, const Vector3& b)
{
  return Vector3(a[0]/b[0], a[1]/b[1], a[2]/b[2]);
}

////////////////////////////////////////////////////////////////////////////////

/// operator for notation Vector3/=Vector3

void
Vector3::operator/= (const Vector3& v)
{
  _data[0] /= v[0];
  _data[1] /= v[1];
  _data[2] /= v[2];
}

////////////////////////////////////////////////////////////////////////////////

/// equal

/*extern*/
bool operator== (const Vector3& a, const Vector3& b)
{
  if (   std::abs(a[0] - b[0]) < GLOOST_MATHTYPE_MIN_EPSILON
      && std::abs(a[1] - b[1]) < GLOOST_MATHTYPE_MIN_EPSILON
      && std::abs(a[2] - b[2]) < GLOOST_MATHTYPE_MIN_EPSILON  )
  {
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////

/// not equal

/*extern*/
bool operator!= (const Vector3& a, const Vector3& b)
{
  if (   std::abs(a[0] - b[0]) < GLOOST_MATHTYPE_MIN_EPSILON
      && std::abs(a[1] - b[1]) < GLOOST_MATHTYPE_MIN_EPSILON
      && std::abs(a[2] - b[2]) < GLOOST_MATHTYPE_MIN_EPSILON  )
  {
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

  ///

/* extern */
std::ostream&
operator<< (std::ostream& os, const Vector3& a)
{
  os << "Vector3(" << std::fixed << a[0] << "," << a[1] << "," << a[2] << ")";
  return os;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace gloost
