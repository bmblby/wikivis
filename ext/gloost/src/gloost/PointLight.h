
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



#ifndef GLOOST_POINTLIGHT_H
#define GLOOST_POINTLIGHT_H

// gloost includes
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/UniformTypes.h>

// system includes


namespace gloost
{


/// wrapper for a openGL point light and base class for all kinds of point lights

class PointLight
{

public:

  /// class constructor
  PointLight();
  virtual ~PointLight();

  Point3& getPosition();
  void setPosition(const Point3& position);
  void setPosition(float x, float y, float z);


  gloost::vec4 getAmbient() const;
  void setAmbient(const float& r, const float& g, const float& b, const float& a);
  void setAmbient(const gloost::vec4& color);

  gloost::vec4 getDiffuse() const;
  void setDiffuse(const float& r, const float& g, const float& b, const float& a);
  void setDiffuse(const gloost::vec4& color);

  gloost::vec4 getSpecular() const;
  void setSpecular(const float& r, const float& g, const float& b, const float& a);
  void setSpecular(const gloost::vec4& color);



  void enableSpot();
  void disableSpot();
  bool isSpotLight();


  void setSpotDirection(const Vector3& spotDir);
  void setSpotExponent(float exponent);

  /// _spotCutoff is 1/2 of the spot lights coneangle (in degrees)
  void setSpotCutoff(float cutoff);

  /// area (in light direction) the light is working
  void setSpotNear(float spotNear);
  void setSpotFar(float spotFar);


 protected:

  gloost::Point3 _position;

  gloost::vec4 _ambient;
  gloost::vec4 _diffuse;
  gloost::vec4 _specular;

  bool _isSpotLight;

  gloost::Vector3 _spotDirection;
  float _spotExponent;
  float _spotCutoff;

  float _spotNear;
  float _spotFar;
};


} // namespace gloost


#endif // GLOOST_POINTLIGHT_H
