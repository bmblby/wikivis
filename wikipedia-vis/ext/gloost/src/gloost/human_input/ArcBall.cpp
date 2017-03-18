
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

#include <gloost/human_input/ArcBall.h>
#include <gloost/Point3.h>

namespace gloost
{
namespace human_input
{

/**
  \class   ArcBall
  \brief   Transforms mouse movement to an ArcBall
  \author  Felix Weiszig, Dynardo GmbH
  \date    September 2015
  \remarks Code was partial taken from Nehe Lessons #48:
           http://nehe.gamedev.net/tutorial/lessons_46__48/16016/
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

ArcBall::ArcBall(float width, float height):
  _startVector(0.0, 0.0, 0.0),
  _endVector(0.0, 0.0, 0.0),
  _screenSizeNormalizerX(1),
  _screenSizeNormalizerY(1)
{
  setScreenSize(width, height);
}

////////////////////////////////////////////////////////////////////////////////

ArcBall::~ArcBall()
{
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class factory
  \remarks ...
*/
/*static*/
ArcBall::shared_ptr
ArcBall::create(float width, float height)
{
  return shared_ptr(new ArcBall(width, height));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
ArcBall::setScreenSize(float width, float height)
{
  // Set adjustment factor for width/height
  _screenSizeNormalizerX = 1.0f / ((width -  1.0f) * 0.5f);
  _screenSizeNormalizerY = 1.0f / ((height - 1.0f) * 0.5f);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the horizontal length of the interation area
  \param   ...
  \remarks ...
*/

float
ArcBall::getScreenSizeX() const
{
  return _screenSizeNormalizerX;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the vertical length of the interation area
  \param   ...
  \remarks ...
*/

float
ArcBall::getScreenSizeY() const
{
  return _screenSizeNormalizerY;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

//Mouse down
void
ArcBall::click(gloost::Point3& point)
{
  // Map the point to the sphere
  _startVector = mapToSphere(point);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Mouse drag, calculate rotation and returns a quaternion as an  gloost::Vector3(x,y,z,w)
  \param   ...
  \remarks ...
*/

gloost::Vector3
ArcBall::drag(const gloost::Point3& currentMouseCoord)
{
  // Map the point to the sphere
  _endVector = mapToSphere(currentMouseCoord);

  // Return the quaternion equivalent to the rotation
  // Compute the vector perpendicular to the begin and end vectors
  gloost::Vector3 perpVector = cross(_startVector, _endVector);
  // Compute the length of the perpendicular vector
  if (perpVector.length() > GLOOST_MATHTYPE_MIN_EPSILON)    //if its non-zero
  {
    return { perpVector[0], perpVector[1], perpVector[2], _startVector*_endVector };
  }
  // The begin and end vectors coincide, so return an identity transform
  return {0.0, 0.0, 0.0, 0.0};
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief  ...
  \param   ...
  \remarks ...
*/

gloost::Vector3
ArcBall::getQuaternionForMouseMovement(const gloost::Point3& p1, const gloost::Point3& p2) const
{
  // Map the point to the sphere
  const auto p1OnSphere = mapToSphere(p1);
  const auto p2OnSphere = mapToSphere(p2);

  // Return the quaternion equivalent to the rotation
  // Compute the vector perpendicular to the begin and end vectors
  gloost::Vector3 perpVector = cross(p1OnSphere, p2OnSphere);
  // Compute the length of the perpendicular vector
  if (perpVector.length() > GLOOST_MATHTYPE_MIN_EPSILON)    //if its non-zero
  {
    return { perpVector[0], perpVector[1], perpVector[2], p1OnSphere*p2OnSphere };
  }

  // The begin and end vectors coincide, so return an identity transform
  return {0.0, 0.0, 0.0, 0.0};
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

gloost::Vector3
ArcBall::mapToSphere(const gloost::Point3& point) const
{
  // Copy paramter into temp point
  gloost::Point3 tempPoint = point;
//  tempPoint[2] = 0.0;

  //Adjust point coords and scale down to range of [-1 ... 1]
  tempPoint[0] =  (tempPoint[0] * _screenSizeNormalizerX) - 1.0f;
  tempPoint[1] =  (tempPoint[1] * _screenSizeNormalizerY) - 1.0f;;


  //Compute the square of the length of the vector to the point from the center
  float length = (tempPoint[0] * tempPoint[0]) + (tempPoint[1]* tempPoint[1]);
  auto  returnVector = gloost::Vector3(0.0,0.0,0.0);

  //If the point is mapped outside of the sphere... (length > radius squared)
  if (length > 1.0f)
  {
    //Compute a normalizing factor (radius / sqrt(length))
    float norm = 1.0f / std::sqrt(length);

    //  Return the "normalized" vector, a point on the sphere
    returnVector[0] = tempPoint[0] * norm;
    returnVector[1] = tempPoint[1] * norm;
    returnVector[2] = 0.0f;
  }
  else    //Else it's on the inside
  {
    //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
    returnVector[0] = tempPoint[0];
    returnVector[1] = tempPoint[1];
    returnVector[2] = std::sqrt(1.0f - length);
  }

  return returnVector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief Mouse drag, calculate rotation and returns a quaternion as an  gloost::Vector3(x,y,z,w)

*/

/*static*/
gloost::Matrix
ArcBall::getRotationFromFromQuaternion(const gloost::Vector3& quat)
{
  float n = (quat[0] * quat[0]) + (quat[1] * quat[1]) + (quat[2] * quat[2]) + (quat[3] * quat[3]);
  float s = (n > 0.0f) ? (2.0f / n) : 0.0f;

  float xs = quat[0] * s;
  float ys = quat[1] * s;
  float zs = quat[2] * s;

  float wx = quat[3] * xs;
  float wy = quat[3] * ys;
  float wz = quat[3] * zs;

  float xx = quat[0] * xs;
  float xy = quat[0] * ys;
  float xz = quat[0] * zs;

  float yy = quat[1] * ys;
  float yz = quat[1] * zs;
  float zz = quat[2] * zs;

  gloost::Matrix resultMatrix;
  resultMatrix.setIdentity();

  resultMatrix[0]  = 1.0f - (yy + zz);
  resultMatrix[1]  =         xy - wz;
  resultMatrix[2]  =         xz + wy;

  resultMatrix[4]  =         xy + wz;
  resultMatrix[5]  = 1.0f - (xx + zz);
  resultMatrix[6]  =         yz - wx;

  resultMatrix[8]  =         xz - wy;
  resultMatrix[9]  =         yz + wx;
  resultMatrix[10] = 1.0f - (xx + yy);
  return resultMatrix;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace human_input
}  // namespace gloost
