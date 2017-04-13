
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
#include <gloost/PointLight.h>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

PointLight::PointLight():
    _position(0.0,0.0,0.0),
    _ambient(),
    _diffuse(),
    _specular(),
    _isSpotLight(false),
    _spotDirection(),
    _spotExponent(),
    _spotCutoff(),
    _spotNear(0.0f),
    _spotFar(10000.0f)
{
  _ambient.r = 0.1;
  _ambient.g = 0.1;
  _ambient.b = 0.1;
  _ambient.a = 1.0;

  _diffuse.r = 0.5;
  _diffuse.g = 0.5;
  _diffuse.b = 0.5;
  _diffuse.a = 1.0;

  _specular.r = 1.0;
  _specular.g = 1.0;
  _specular.b = 1.0;
  _specular.a = 1.0;
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

PointLight::~PointLight()
{}


///////////////////////////////////////////////////////////////////////////////


  ///

Point3&
PointLight::getPosition()
{
  return _position;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setPosition(const Point3& position)
{
  _position[0] = (float) position[0];
  _position[1] = (float) position[1];
  _position[2] = (float) position[2];
  _position[3] = (float) 1.0;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setPosition(float x, float y, float z)
{
  _position[0] = x;
  _position[1] = y;
  _position[2] = z;
  _position[3] = 1.0;
}


///////////////////////////////////////////////////////////////////////////////


  ///

gloost::vec4
PointLight::getAmbient() const
{
  return _ambient;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setAmbient(const float& r, const float& g, const float& b, const float& a)
{
  _ambient = vec4(r,g,b,a);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setAmbient(const gloost::vec4& color)
{
  _ambient = color;
}


///////////////////////////////////////////////////////////////////////////////


  ///

gloost::vec4
PointLight::getDiffuse() const
{
  return _diffuse;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setDiffuse(const float& r, const float& g, const float& b, const float& a)
{
  _diffuse = vec4(r,g,b,a);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setDiffuse(const gloost::vec4& color)
{
  _diffuse = color;
}


///////////////////////////////////////////////////////////////////////////////


  ///

gloost::vec4
PointLight::getSpecular() const
{
  return _specular;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setSpecular(const float& r, const float& g, const float& b, const float& a)
{
  _specular = vec4(r,g,b,a);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setSpecular(const gloost::vec4& color)
{
  _specular = color;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::enableSpot()
{
  _isSpotLight = true;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::disableSpot()
{
  _isSpotLight = false;
}


///////////////////////////////////////////////////////////////////////////////


  ///

bool
PointLight::isSpotLight()
{
  return _isSpotLight;
}


///////////////////////////////////////////////////////////////////////////////

void
PointLight::setSpotDirection(const Vector3& spotDir)
{
  _spotDirection = spotDir.normalized();
}

///////////////////////////////////////////////////////////////////////////////


void
PointLight::setSpotExponent(float exponent)
{
  _spotExponent = exponent;
}

///////////////////////////////////////////////////////////////////////////////


void
PointLight::setSpotCutoff(float cutoff)
{
  _spotCutoff = cutoff;
}

///////////////////////////////////////////////////////////////////////////////

void
PointLight::setSpotNear(float spotNear)
{
  _spotNear = spotNear;
}

///////////////////////////////////////////////////////////////////////////////

void
PointLight::setSpotFar(float spotFar)
{
  _spotFar = spotFar;
}

///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
