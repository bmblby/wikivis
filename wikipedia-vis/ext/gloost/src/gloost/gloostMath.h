
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




#ifndef H_GLOOST_MATH
#define H_GLOOST_MATH

// cpp includes
#include <sstream>
#include <cmath>
#include <stdlib.h>

// gloost includes
#include  <gloost/UniformTypes.h>
#include  <gloost/Vector3.h>
#include  <gloost/Vector2.h>

/// init rand
#ifndef RAND_MAX
  #define RAND_MAX 2147483647
#endif

namespace gloost
{

namespace math
{

/// define PI
static const mathType PI    = 3.1415926535897932;
static const mathType EULER = 2.7182818284590452;

///////////////////////////////////////////////////////////////////////////////

  /// returns a "random" value between 0.0 ... 1.0

inline float
frand ()
{
  return (rand()/((float)RAND_MAX));
}

///////////////////////////////////////////////////////////////////////////////

  /// returns a "random" value between -1.0 ... 1.0

inline float
crand ()
{
  return (rand()/((float)RAND_MAX))*2.0f - 1.0f;
}


///////////////////////////////////////////////////////////////////////////////

  /// returns a "random" value between fMin ... fMax

template <class T>
inline T
getRandomMinMax( T fMin, T fMax )
{
  float fRandNum = (float)rand () / RAND_MAX;
  return fMin + (fMax - fMin) * fRandNum;
}


///////////////////////////////////////////////////////////////////////////////

  /// returns true or false by a chance of 50%

inline bool chance(float probability = 0.5)
{
  if (frand() < probability)
  {
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////

  /// returns minimum of two values

template <class T>
inline T
min ( T a, T b)
{
  if ( a > b)
  {
    return b;
  }

  return a;
}


///////////////////////////////////////////////////////////////////////////////

  /// returns minimum of two values

template <class T>
inline T
min ( T a, T b, T c)
{
  return min(min(a,b), c);
}


///////////////////////////////////////////////////////////////////////////////

  /// returns maximum of two values

template <class T>
inline T
max ( T a, T b)
{
  if ( a > b)
  {
    return a;
  }

  return b;
}


///////////////////////////////////////////////////////////////////////////////

  /// returns maximum of two values

template <class T>
inline T
max ( T a, T b, T c)
{
  return max(max(a,b), c);
}


///////////////////////////////////////////////////////////////////////////////

//  /// returns maximum of two values
//
//template <class T>
//void
//max ( T& a, T b)
//{
//  if ( a > b)
//  {
//    return;
//  }
//
//  return a = b;
//}


///////////////////////////////////////////////////////////////////////////////

  /// orders two values so that a contains the smaller value and b the bigger one

template <class T>
inline
void
swapToIncreasing ( T& a, T& b)
{
  if (a > b)
  {
    T temp = a;
    a = b;
    b = temp;
  }
}


///////////////////////////////////////////////////////////////////////////////

  /// orders two values so that a contains the bigger value and b the smaller one

template <class T>
void
swapToDecreasing ( T& a, T& b)
{
  if (a < b)
  {
    T temp = a;
    a = b;
    b = temp;
  }
}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   orders the components of two Point3 so that the first one will be PMin and
           the second one PMax of an bounding box
  \param   ...
  \remarks ...
*/

inline
void
pointsToPMinPmax (Point3& p1, Point3& p2)
{
  gloost::Point3 pmin;
  gloost::Point3 pmax;

  for (unsigned i=0; i!=3; ++i)
  {
    if (p1[i] < p2[i])
    {
      pmin[i] = p1[i];
      pmax[i] = p2[i];
    }
    else
    {
      pmin[i] = p2[i];
      pmax[i] = p1[i];
    }
  }

  p1 = pmin;
  p2 = pmax;
}


///////////////////////////////////////////////////////////////////////////////

  /// returns 1 if a is >= 0, else -1

template <class T>
inline
int
sign ( T a )
{
  if ( a < 0)
  {
    return -1;
  }

  return 1;
}


///////////////////////////////////////////////////////////////////////////////

  /// clamps a value between min and max

template <class T>
inline
T
clamp ( T value, T min, T max )
{
  if (value < min)
  {
    return min;
  }
  else if (value > max)
  {
    return max;
  }
  return value;
}


///////////////////////////////////////////////////////////////////////////////

  /// returns true if value is between min and max

template <class T>
inline
bool
between ( T value, T min, T max )
{
  if (value > min && value < max)
  {
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

  /// returns true if value is between or equal to min and max

template <class T>
inline
bool
betweenOrEqual ( T value, T min, T max )
{
  if (value >= min && value <= max)
  {
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

  /// loop a value between min and max

template <class T>
inline
T
loop ( T value, T min, T max )
{
  if (value < min)
  {
    return max - abs(value-min);
  }
  else if (value > max)
  {
    return value - abs(max - min);
  }
  return value;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   normalizes a value between min and max to a value between 0.0 and 1.0
  \remarks ...
*/

template <class T>
inline
T
normalize ( T value, T min, T max )
{
  return (value - min) / (max - min);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   inflates a normalized value between 0.0 and 1.0 to min-max-space (so to speak)
  \remarks ...
*/

template <class T>
inline
T
unnormalize ( T normalizedValue, T min, T max )
{
  return (normalizedValue*(max-min)+min);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   normalizes a value between min and max to a value between 0.0 and 1.0
  \param   value
  \param   min minimal value
  \param   maxMinusMinInv 1.0f/max-min
  \remarks ...
*/

template <class T>
inline
T
normalizeFaster ( T value, T min, T maxMinusMinInv )
{
  return (value - min) * maxMinusMinInv;
}

///////////////////////////////////////////////////////////////////////////////

/* (this function is templated, so if you want float precision give floats!!!) */

  /// converts degree to rad

template <class T>
inline
T
deg2rad (const T& a)
{
  return (T)(a * PI / (T)180.0);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   converts degree to rad
  \remarks (this function is templated, so if you want float precision give floats!!!)
*/

template <class T>
inline
T
rad2deg (T a)
{
  return (T)(a * (T)180.0 / PI);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief coverts a angle in a vector
*/

template <class T>
inline
Vector2
angle2Vector(T angle)
{
  return Vector2(cos(angle*0.0174532925199), sin(angle*0.0174532925199));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief coverts a angle in a vector
*/

template <class T>
inline
gloost::Vector3
angle2Vector3xy(T angle)
{
  return gloost::Vector3(cos(angle*0.0174532925199), sin(angle*0.0174532925199), 0.0);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief coverts a angle in a vector
*/

template <class T>
inline
gloost::Vector3
angle2Vector3xz(T angle)
{
  return gloost::Vector3(cos(angle*0.0174532925199), 0.0, sin(angle*0.0174532925199));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief coverts a 2D vector in to an angle
*/

inline
float
vector2angleRad(float deltaH, float deltaV, float epsilon = 0.0000001)
{
  float  the_angle = 0.0;

  if ( abs(deltaH-epsilon) > epsilon )
  {
    float slope = deltaV / deltaH;
    the_angle = atan (slope);
    if (deltaH  < 0.0)
    {
      the_angle = the_angle + PI;
    }
  }
  else if (deltaV > 0.0)
  {
    the_angle = PI / 2.0;
  }
  else if (deltaV < 0.0)
  {
    the_angle = (3.0 * PI) / 2.0;
  }
  else
  {
    the_angle = 0.0;
  }

  return the_angle;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief coverts a 2D vector in to an angle
*/

inline
float
vector2angle(float deltaH, float deltaV)
{
  float the_angle = 0.0;
  static mathType epsilon = 0.000000001;

  if ( abs(deltaH-epsilon) > epsilon )
  {
    float slope = deltaV / deltaH;
    the_angle = atan (slope);
    if (deltaH  < 0)
    {
      the_angle = the_angle + PI;
    }
  }
  else if (deltaV > 0)
  {
    the_angle = PI / 2.0;
  }
  else if (deltaV < 0)
  {
    the_angle = (3.0 * PI) / 2.0;
  }
  else
  {
    the_angle = 0.0;
  }

  the_angle = (the_angle * 180.0) / PI;

  return the_angle;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief coverts a 2D vector in to an angle
*/

inline
float
vector2angle(vec2 vector)
{
  return vector2angle(vector.u, vector.v);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief the modulo or "%" operator for all scalar types
*/

template <class T>
T
fmod(T a, T b)
{
  int result = static_cast<int>( a / b );
  return a - static_cast<double>( result ) * b;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief the modulo or "%" operator for all scalar types
*/

template <class T>
T
fract(T value)
{
  return value - long(value);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns TRUE if two lines are intersect each other
*/

inline
bool
lineIntersect2D(const Vector2& line1P1, const Vector2& line1P2,
                const Vector2& line2P1, const Vector2& line2P2 )
{
  // direction vectors of both lines
  gloost::Vector2 d1 = line1P2 - line1P1;
  gloost::Vector2 d2 = line2P2 - line2P1;
  float div = d1[0] * d2[1] - d1[1] * d2[0];
  float t = -(line1P1[0] * d2[1] - line1P1[1] * d2[0] - line2P1[0] * d2[1] + line2P1[1] * d2[0]) / div;
  float s = -(line1P1[0] * d1[1] - line1P1[1] * d1[0] + d1[0] * line2P1[1] - d1[1] * line2P1[0]) / div;
  // or u1.multiply(s) u0.plus(u1);
  d1 = d1 * t;
  // der Punkt wo sie sich schneiden
//  gloost::Vector2 p = line1P1 + d1;
  if ((t > 0 && s > 0) && (t < 1 && s < 1))
  {
     return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief converts a value in byte into mega byte
  \param byteValue a value in bytes
  \remarks ...
*/

inline
float
convertByteToMegaByte(float byteValue)
{
  return byteValue/1048576.0f;
}

/////////////////////////////////////////////////////////////////////////////////

/**
  \brief converts a value in bit into mega byte
  \param byteValue a value in bytes
  \remarks ...
*/

inline
float
convertBitToMegaByte(float bitValue)
{
  return bitValue/8388608.0f;
}

/////////////////////////////////////////////////////////////////////////////////

///**
//  \brief gauss
//  \param   ...
//  \remarks ...
//*/
//
//inline
//float
//gauss(float x, float a=1.0, float b=0, float c=0.5)
//{
//  return a*pow(EULER, -( ((x-b)*(x-b)) )/(2*c*c) );
//}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   interpolates linearly between two values a and b
  \param a first control point
  \param b second control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

template <class genericType, class timeType>
inline genericType
interpolateLinear(genericType a, genericType b, timeType t)
{
	return a * ((genericType)1.0 - t) + b * t;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   interpolates linearly between two points a and b
  \param a first control point
  \param b second control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline gloost::Point3
interpolateLinear(const gloost::Point3& a, const gloost::Point3& b, mathType t)
{
	return gloost::Point3( interpolateLinear(a[0], b[0], t),
          						   interpolateLinear(a[1], b[1], t),
					          	   interpolateLinear(a[2], b[2], t));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   interpolates quadratic between two values a and c
  \param a first control value
  \param b second control value
  \param c third control value
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

template <class genericType, class timeType>
inline genericType
interpolateQuadratic(genericType a, genericType b, genericType c, timeType t)
{
	genericType h = (genericType)1.0 - t;
	return a * h * h + b * (genericType)2 * h * t + c * t * t;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   interpolates quadratic between two points a and c
  \param a first control point
  \param b second control point
  \param c third control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline gloost::Point3
interpolateQuadratic(const gloost::Point3& a, const gloost::Point3& b, const gloost::Point3& c, mathType t)
{
	return gloost::Point3( interpolateQuadratic(a[0], b[0], c[0], t),
                         interpolateQuadratic(a[1], b[1], c[1], t),
                         interpolateQuadratic(a[2], b[2], c[2], t));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   interpolates cubic between 2 values a and d
  \param a first control value
  \param b second control value
  \param c third control value
  \param d last control value
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

template <class genericType, class timeType>
inline genericType
interpolateCubic(genericType a, genericType b, genericType c, genericType d,  timeType t)
{
	timeType h  = (timeType)1.0 - t;
	return a * h*h*h + b * (timeType)3 * h * h * t + c * (timeType)3 * h * t * t + d * t * t * t;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   interpolates cubic between two points a and d
  \param a first control point
  \param b second control point
  \param c third control point
  \param d last control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline gloost::Point3
interpolateCubic(const gloost::Point3& a, const gloost::Point3& b,
                 const gloost::Point3& c, const gloost::Point3& d, mathType t)
{
	return gloost::Point3( interpolateCubic(a[0], b[0], c[0], d[0], t),
                         interpolateCubic(a[1], b[1], c[1], d[1], t),
                         interpolateCubic(a[2], b[2], c[2], d[2], t));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   performace a hermite interpolation between edge0 and edge1, is equivalent to glsl smoothstep
  \remarks returns a value between 0 and 1.0.
*/

inline float
smoothstep(float edge0, float edge1, float value)
{
  float t = clamp((value - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  return t * t * (3.0f - 2.0f * t);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the value of the gauss distribution function
  \param x value
  \param ro standard deviation
  \param mue mean or expectation of the distribution (and also its median and mode)

  \remarks http://en.wikipedia.org/wiki/Normal_distribution
*/

inline double
gaussDistrib (double x, double ro=1.0, double mue=0.0)
{
  static const double sqrtOf2PI = sqrt(2.0*PI);

  const double term1 = 1.0/(ro*sqrtOf2PI);
  double term2 = pow(EULER, -1.0*( (x-mue)*(x-mue))/(2.0*ro*ro) );

  return term1*term2;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the distance*distance between a line and point in 3D
  \remarks ...
*/

inline gloost::mathType
distance2_line_point(const gloost::Point3& point,
                     const gloost::Point3& lineP1,
                     const gloost::Point3& lineP2)
{
  const gloost::Vector3 v = lineP2 - lineP1;
  const gloost::Vector3 w = point  - lineP1;

  const mathType c1 = w*v;
  const mathType c2 = v*v;
  const mathType b  = c1 / c2;

  const gloost::Point3 Pb = lineP1 + b * v;
  return (point - Pb)*(point - Pb);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the distance between a line and point in 3D
  \remarks ...
*/

inline gloost::mathType
distance_line_point(const gloost::Point3& point,
                    const gloost::Point3& lineP1,
                    const gloost::Point3& lineP2)
{
  return sqrt(distance2_line_point(point, lineP1, lineP2));
}


///////////////////////////////////////////////////////////////////////////////

#if 0
/**
  \brief   returns 1/sqrt(number), calculated by one newton iteration
  \remarks This was suposetly taken from the Quake 3 source but was pioniered by Greg Walsh.
           https://www.beyond3d.com/content/articles/15/
*/

inline
float
qInvSqrt( float value )
{
  float xhalf = 0.5f*value;
  int i = *(int*)&value;
  i = 0x5f3759df - (i>>1);
  value = *(float*)&i;
  value = value*(1.5f - xhalf*value*value);
  return value;
}
#endif // 0

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   discretizes a value between min and max to numLevels
  \remarks ...
*/

template <class Type>
inline Type
discretize(Type value, int resolution, Type min, Type max)
{
  const auto steps = resolution-1;
  const auto stepWidth  = (max-min)/(float)steps;
  return min + stepWidth * int((value-min)/stepWidth);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the serial index of a serialized 2d array
  \remarks posX,posY position within the 3d structure
  \remarks resX,resY resolution of the x 3d structure
*/

template <class Type>
Type
coord2dToSerialIndex(Type posX, Type posY,
                     Type resX)
{
  return posX + posY*resX;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the serial index of a serialized 3d array
  \remarks posX,posY,posZ position within the 3d structure
  \remarks resX,resY,resZ resolution of the x 3d structure
*/

template <class Type>
Type
coord3dToSerialIndex(Type posX, Type posY, Type posZ,
                     Type resX, Type resY)
{
  return posX + posY*resX + posZ*resX*resY;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   maps a serial index of a 2d volume container to a x,y position
*/

inline
Vector2
serialIndexToCoord2d(unsigned serialIndex, unsigned resX)
{
  auto y = std::floor( serialIndex/resX );
  auto x = serialIndex - y*resX;
  return {x,y};
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   maps a serial index of a 3d volume container to a x,y,z position
*/

inline
Vector3
serialIndexToCoord3d(unsigned serialIndex, unsigned resX, unsigned resY)
{
  auto z = std::floor( serialIndex/(resX*resY) );
  auto y = std::floor( (serialIndex-z*resX*resY)/resX );
  auto x = serialIndex - y*resX - z*resX*resY;
  return {x,y,z};
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   maps a value within the range from fromMin to fromMax to a value within the range toMin to toMax
*/

template <class SourceType, class DestinationType>
DestinationType
map(SourceType value,
    SourceType fromMin, SourceType fromMax,
    DestinationType toMin, DestinationType toMax)
{
  return (DestinationType)(value - fromMin)*(toMax - toMin)/(fromMax - fromMin) + toMin;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a RGBa color from a HSVa definition
  \remarks hue, saturation and value should be given in range 0.0 ... 1.0
*/

inline
vec4
hsvToRgb(float hue, float saturation, float value, float alpha)
{
  float r, g, b;
  int   i = floor(hue * 6);
  float f = hue * 6 - i;
  float p = value * (1 - saturation);
  float q = value * (1 - f * saturation);
  float t = value * (1 - (1 - f) * saturation);

  switch(i % 6)
  {
    case 0: r = value, g = t, b = p; break;
    case 1: r = q, g = value, b = p; break;
    case 2: r = p, g = value, b = t; break;
    case 3: r = p, g = q, b = value; break;
    case 4: r = t, g = p, b = value; break;
    case 5: r = value, g = p, b = q; break;
  }

  return vec4(r,g,b,alpha);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a HSL color from a RGB definition
  \remarks
*/

inline
vec4
rgbToHsl(vec4 color)
{
	float fmin = gloost::math::min(gloost::math::min(color.r, color.g), color.b);    //Min. value of RGB
	float fmax = gloost::math::max(gloost::math::max(color.r, color.g), color.b);    //Max. value of RGB
	float delta = fmax - fmin;             //Delta RGB value

	vec4 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
	hsl.b = (fmax + fmin) / 2.0; // Luminance

	if (delta == 0.0)		//This is a gray, no chroma...
	{
		hsl.r = 0.0;	// Hue
		hsl.g = 0.0;	// Saturation
	}
	else                                    //Chromatic data...
	{
		if (hsl.b < 0.5)
			hsl.g = delta / (fmax + fmin); // Saturation
		else
			hsl.g = delta / (2.0 - fmax - fmin); // Saturation

		float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
		float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
		float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;

		if (color.r == fmax )
			hsl.r = deltaB - deltaG; // Hue
		else if (color.g == fmax)
			hsl.r = (1.0 / 3.0) + deltaR - deltaB; // Hue
		else if (color.b == fmax)
			hsl.r = (2.0 / 3.0) + deltaG - deltaR; // Hue

		if (hsl.r < 0.0)
			hsl.r += 1.0; // Hue
		else if (hsl.r > 1.0)
			hsl.r -= 1.0; // Hue
	}
	return hsl;
}

///////////////////////////////////////////////////////////////////////////////


}  // namespace math

}  // namespace gloost


#endif // #ifndef H_GLOOST_MATH


