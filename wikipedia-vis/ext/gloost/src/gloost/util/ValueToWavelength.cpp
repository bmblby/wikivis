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
#include <gloost/util/ValueToWavelength.h>
#include <gloost/gloostMath.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace util
{


/**
  \class   ValueToWavelength

  \brief   converts a value between min...max to a color of the visible spectrum

  \author  Stephan Beck, adopted by Felix Weiszig
  \date    May 2013
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   converts a value between min...max to a color of the visible spectrum
  \param   ...
  \remarks ...
*/

/*static*/
vec4
ValueToWavelength::toWavelengthColor(float value, float min, float max)
{
  const float wavelength = getWaveLengthFromDataPoint(value, min, max);
  const vec4 rgba = wavelengthToRGB(wavelength);

//  std::cerr << std::endl << "(toWavelengthColor): " << rgba;

  static const float fac = 1.0f/255.0f;
  return vec4(rgba.r * fac, rgba.g * fac, rgba.b * fac, 1.0f);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*static*/
float
ValueToWavelength::getWaveLengthFromDataPoint(float value, float min, float max)
{
  static const float MinVisibleWavelength = 380.0f;//350.0;
  static const float MaxVisibleWavelength = 780.0f;//650.0;
  //Convert data value in the range of MinValues..MaxValues to the
  //range 350..650
  return (value - min) / (max-min)
          * (MaxVisibleWavelength - MinVisibleWavelength) + MinVisibleWavelength;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*static*/
vec4
ValueToWavelength::wavelengthToRGB(float wavelength)
{
  float Blue;
  float factor;
  float Green;
  float Red;

  if(380.0f <= wavelength && wavelength <= 440.0f)
  {
    Red   = -(wavelength - 440.0f) / (440.0f - 380.0f);
    Green = 0.0f;
    Blue  = 1.0f;
  }
  else if(440.0f < wavelength && wavelength <= 490.0f)
  {
    Red   = 0.0f;
    Green = (wavelength - 440.0f) / (490.0f - 440.0f);
    Blue  = 1.0f;
  }
  else if(490.0f < wavelength && wavelength <= 510.0f)
  {
    Red   = 0.0f;
    Green = 1.0f;
    Blue  = -(wavelength - 510.0f) / (510.0f - 490.0f);
  }
  else if(510.0f < wavelength && wavelength <= 580.0f)
  {
    Red   = (wavelength - 510.0f) / (580.0f - 510.0f);
    Green = 1.0f;
    Blue  = 0.0f;
  }
  else if(580.0f < wavelength && wavelength <= 645.0f)
  {
    Red   = 1.0f;
    Green = -(wavelength - 645.0f) / (645.0f - 580.0f);
    Blue  = 0.0;
  }
  else if(645.0f < wavelength && wavelength <= 780.0f)
  {
    Red   = 1.0f;
    Green = 0.0f;
    Blue  = 0.0f;
  }
  else
  {
    Red   = 0.0f;
    Green = 0.0f;
    Blue  = 0.0f;
  }
  if(380.0f <= wavelength && wavelength <= 420.0f)
  {
    factor = 0.3f + 0.7f*(wavelength - 380.0f) / (420.0f - 380.0f);
  }
  else if(420.0f < wavelength && wavelength <= 701.0f)
  {
    factor = 1.0f;
  }
  else if(701.0f < wavelength && wavelength <= 780.0f)
  {
    factor = 0.3f + 0.7f*(780.0f - wavelength) / (780.0f - 701.0f);
  }
  else
  {
    factor = 0.0f;
  }

  return vec4(adjust(Red,   factor),
              adjust(Green, factor),
              adjust(Blue,  factor),
              1.0f);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

/*static*/
unsigned
ValueToWavelength::adjust(float colorComponent, float factor)
{
  static float gamma        = 0.8f;
  static float intensityMax = 255.0f;

  if (colorComponent == 0.0f)
  {
    return 0u;
  }
//  int res = round(intensityMax * pow(colorComponent * factor, gamma));
  int res = floor(intensityMax * pow(colorComponent * factor, gamma) + 0.5 );
  return (unsigned)math::min(255, math::max(0, res));
}


////////////////////////////////////////////////////////////////////////////////





} // namespace util
} // namespace gloost


