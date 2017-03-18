
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

// cpp includes
#include <cstring>

namespace gloost
{

/*static*/ Point3 Point3::origin = Point3(0.0,0.0,0.0);

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

Point3::Point3():
  _data()
{
//  _data[0]=0.0;
//  _data[1]=0.0;
//  _data[2]=0.0;
//  _data[3]=1.0;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

Point3::Point3 (const Vector3& v):
  _data ()
{
  memcpy(_data, v.data(), sizeof(mathType)*3);
  _data[3] = 1.0;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

Point3::Point3 (const Point3& p):
  _data ()
{
  memcpy(_data, p._data, sizeof(mathType)*4);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

Point3::Point3(const mathType a, const mathType b, const mathType c):
    _data()
{
    _data[0] = a;
    _data[1] = b;
    _data[2] = c;
    _data[3] = 1.0;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

Point3::Point3(const mathType  a,const mathType b,const mathType c, const mathType w)
    : _data()
{
    _data[0]=a;
    _data[1]=b;
    _data[2]=c;
    _data[3]=w;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

//Point3::Point3(std::ifstream& in)
//    : _data()
//{
//  in >> _data[0];
//  in >> _data[1];
//  in >> _data[2];
//  _data[3] = 1.0;
//}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

mathType&
Point3::operator[](unsigned int a)
{
  return (4 < a) ? _data[0] : _data[a];
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

const mathType&
Point3::operator[] (unsigned int a) const
{
  return (4 < a) ? _data[0] : _data[a];
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/
//Point3::operator bool() const
//{
//  return distanceToOrigin2() > GLOOST_MATHTYPE_MIN_EPSILON;
//}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param
  \remarks
*/

Point3&
Point3::operator= (const Point3& a)
{
  memcpy(_data, a._data, sizeof(mathType)*4);
  return *this;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   length of the vector from the origin to the point
  \param
  \remarks
*/

mathType
Point3::distanceToOrigin() const
{
  return (sqrt ((_data[0]*_data[0])+
                (_data[1]*_data[1])+
                (_data[2]*_data[2])+
                (_data[3]*_data[3])));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   quadratic length of the vector from the origin to the point
  \param
  \remarks
*/

mathType
Point3::distanceToOrigin2() const
{
  return ((_data[0]*_data[0])+
          (_data[1]*_data[1])+
          (_data[2]*_data[2])+
          (_data[3]*_data[3]));
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a copy of this vector with components converted to int
  \param
  \remarks
*/

Point3
Point3::int_copy() const
{
  return Point3(int(_data[0]), int(_data[1]), int(_data[2]));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   get pointer to an array
  \param
  \remarks
*/

const mathType*
Point3::data() const
{
  return _data;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   get pointer to an array
  \param
  \remarks
*/

mathType*
Point3::data()
{
  return _data;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   operator for notation Point3*mathType
  \param
  \remarks
*/

/*extern*/
Point3
operator* (const Point3& p, const mathType s)
{
  return Point3(p[0]*s, p[1]*s, p[2]*s);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   operator for notation mathType*Point3
  \param
  \remarks
*/

/*extern*/
Point3
operator* (const mathType s, const Point3& p)
{
  return Point3(p[0]*s, p[1]*s, p[2]*s);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Point3 + Vector3
  \param
  \remarks
*/

/*extern*/
Point3 operator+(const Point3& a, const Vector3& b)
{
  return Point3(a[0] + b[0],
                a[1] + b[1],
                a[2] + b[2]);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief    operator for notation Point3+=Vector3
  \param
  \remarks
*/

void
Point3::operator+=(const Vector3& b)
{
  _data[0] += b[0];
  _data[1] += b[1];
  _data[2] += b[2];
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Point3 + Vector3
  \param
  \remarks
*/

/*extern*/
Point3 operator+(const Point3& a, const Point3& b)
{
  return Point3(a[0] + b[0],
                a[1] + b[1],
                a[2] + b[2]);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Point3 + Vector3
  \param
  \remarks
*/

///*extern*/
//Point3 operator+=(const Point3& b)
//{
//  _data[0] += b[0];
//  _data[1] += b[1];
//  _data[2] += b[2];
//}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Point3 - Vector3
  \param
  \remarks
*/

/*extern*/
Point3 operator-(const Point3& a, const Vector3& b)
{
  return Point3(a[0] - b[0],
                a[1] - b[1],
                a[2] - b[2]);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   operator for notation Point3-=Vector3
  \param
  \remarks
*/

void
Point3::operator-=(const Vector3& b)
{
  _data[0] -= b[0];
  _data[1] -= b[1];
  _data[2] -= b[2];
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief    operator for notation Point3*=mathType
  \param
  \remarks
*/

void
Point3::operator*= (const mathType& s)
{
  _data[0] *= s;
  _data[1] *= s;
  _data[2] *= s;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief    operator for notation Point3/=mathType
  \param
  \remarks
*/

void
Point3::operator/= (const mathType& s)
{
  const mathType d = 1.0/s;
  _data[0] *= d;
  _data[1] *= d;
  _data[2] *= d;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   switches the values of two axis
  \remarks ...
*/

void
Point3::flipAxis(unsigned axis1, unsigned axis2)
{
  const auto tmp = _data[axis1];
  _data[axis1]   = _data[axis2];
  _data[axis2]   = tmp;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief    equal
  \param
  \remarks
*/

/*extern*/
bool operator== (const Point3& a, const Point3& b)
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

/**
  \brief    not equal
  \param
  \remarks
*/

/*extern*/
bool operator!= (const Point3& a, const Point3& b)
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

/**
  \brief    out stream
  \param
  \remarks
*/

/* extern */
std::ostream&
operator<< (std::ostream& os, const Point3& a)
{
  os << "Point3( " << std::fixed << a[0] << "," << a[1] << "," << a[2] << " )";
  return os;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gloost
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

