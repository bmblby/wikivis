
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
#include <gloost/animation/Keyframe.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

namespace animation
{


///////////////////////////////////////////////////////////////////////////////


 // class constructor

Keyframe::Keyframe():
  _key()
{
  reset();
}


///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Keyframe::Keyframe(double value, double time):
  _key()
{

  reset();

  _key[GLOOST_KEYFRAME_VALUE] = value;
  _key[GLOOST_KEYFRAME_TIME]  = time;
}



///////////////////////////////////////////////////////////////////////////////


 /// class constructor

Keyframe::Keyframe ( double value,
                     double time,
                     double v1,
                     double tension,
                     double continuity,
                     double bias,
                     double v2,
                     double v3,
                     double v4 ):
  _key()
{
  reset();
  _key[GLOOST_KEYFRAME_VALUE]      = value;
  _key[GLOOST_KEYFRAME_TIME]       = time;
  _key[GLOOST_KEYFRAME_NONE1]      = v1;
  _key[GLOOST_KEYFRAME_TENSION]    = tension;
  _key[GLOOST_KEYFRAME_CONTINUITY] = continuity;
  _key[GLOOST_KEYFRAME_BIAS]       = bias;
  _key[GLOOST_KEYFRAME_NONE2]      = v2;
  _key[GLOOST_KEYFRAME_NONE3]      = v3;
  _key[GLOOST_KEYFRAME_NONE4]      = v4;
}



///////////////////////////////////////////////////////////////////////////////


/// class constructor

Keyframe::Keyframe(const Keyframe& key ):
  _key()
{
  for (int i=0; i != 9; ++i)
  {
    _key[i] = key._key[i];
  }
}


///////////////////////////////////////////////////////////////////////////////


/// class destructor

Keyframe::~Keyframe()
{

}


///////////////////////////////////////////////////////////////////////////////



double
Keyframe::operator[] (unsigned int a) const
{
  return (9 < a) ? _key[0] : _key[a];
}



///////////////////////////////////////////////////////////////////////////////


 /// reset key to zero and remove it from chain

void
Keyframe::reset ( )
{
  _key[0] = 0.0;
  _key[1] = 0.0;
  _key[2] = 0.0;
  _key[3] = 0.0;
  _key[4] = 0.0;
  _key[5] = 0.0;
  _key[6] = 1.0;
  _key[7] = 1.0;
  _key[8] = 1.0;
}


///////////////////////////////////////////////////////////////////////////////


 /// set value parameter

void
Keyframe::setValue (double value)
{
  _key[GLOOST_KEYFRAME_VALUE] = value;
}


///////////////////////////////////////////////////////////////////////////////


 /// get value parameter

double
Keyframe::getValue () const
{
  return _key[GLOOST_KEYFRAME_VALUE];
}


///////////////////////////////////////////////////////////////////////////////


 /// set the time

void
Keyframe::setTime (double value)
{
  _key[GLOOST_KEYFRAME_TIME] = value;
}


///////////////////////////////////////////////////////////////////////////////


 /// get time parameter

double
Keyframe::getTime () const
{
  return _key[GLOOST_KEYFRAME_TIME];
}


///////////////////////////////////////////////////////////////////////////////


/* extern */
std::ostream&
operator<< (std::ostream& os, const gloost::animation::Keyframe& key)
{
  os << std::endl << "Keyframe {" << std::endl
     << "    value      : " << key[GLOOST_KEYFRAME_VALUE] << std::endl
     << "    time       : " << key[GLOOST_KEYFRAME_TIME] << std::endl
     << "               : " << key[2] << std::endl
     << "    tension    : " << key[GLOOST_KEYFRAME_TENSION] << std::endl
     << "    continuity : " << key[GLOOST_KEYFRAME_CONTINUITY] << std::endl
     << "    bias       : " << key[GLOOST_KEYFRAME_BIAS] << std::endl
     << "               : " << key[6] << std::endl
     << "               : " << key[7] << std::endl
     << "               : " << key[8] << std::endl
     << " }";
  return os;
}


///////////////////////////////////////////////////////////////////////////////





} // namespace animation
} // namespace gloost










