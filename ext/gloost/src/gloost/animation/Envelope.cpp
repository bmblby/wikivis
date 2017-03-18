
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
#include <gloost/animation/Envelope.h>


// cpp includes
#include <string>
#include <fstream>
#include <iostream>


namespace gloost
{
namespace animation
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

Envelope::Envelope(float framerate):
  _filename(),
  _keys(),
  _framerate(framerate)
{
	clear();
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

Envelope::~Envelope()
{
	_keys.clear();
}


///////////////////////////////////////////////////////////////////////////////


  /// clears the envelope and ads a key at time 0 with value 0
void
Envelope::clear()
{
	_keys.clear();
	_keys.push_back(Keyframe());
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// load a newtek lightwave 9DOF motion File

void
Envelope::loadLightwaveEnvelope(std::string filename, float framerate)
{

  _filename  = filename;
  _framerate = framerate;


  // open motion file
  std::ifstream filestream( _filename.c_str() );

  if (!filestream)
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in Envelope::loadLightwaveEnvelope(std::string filename, float framerate):" ;
		std::cout << std::endl << "         Unable to open envelope file\"" << filename <<  "\"";
		std::flush(std::cout);
    return;

  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "Message from Envelope::loadLightwaveEnvelope(std::string filename, float framerate):" ;
		std::cout << std::endl << "             Loading \"" << filename <<  "\"";
		std::flush(std::cout);
  }


  // Read the file
  std::string myWord;

  int keycounter   = 0;

  while (filestream >> myWord)
  {


    // Load a Key
    if ( myWord == "Key" )
    {
      double v0, v1, v2, v3, v4, v5, v6, v7, v8;

      filestream >> v0 >> v1 >> v2
                 >> v3 >> v4 >> v5
                 >> v6 >> v7 >> v8;


      addKeyframe( Keyframe (v0, v1*_framerate, v2,
                             v3, v4, v5,
                             v6, v7, v8 ));

      keycounter++;
    }
  }

  filestream.close();


  std::cout << std::endl;
  std::cout << std::endl << "Message from Envelope::loadLightwaveEnvelope(std::string filename, float framerate):" ;
  std::cout << std::endl << "            " << keycounter << " Keys loaded from" << _filename << ".";
  std::flush(std::cout);
}



///////////////////////////////////////////////////////////////////////////////


  /// save a newtek lightwave envelope File

void
Envelope::saveLightwaveEnvelope(std::string filename)
{

  std::ofstream filestream;
  filestream.open(filename.c_str());


  if (!filestream)
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in Envelope::saveLightwaveEnvelope(std::string filename):" ;
		std::cout << std::endl << "         Unable to open envelope file\"" << filename <<  "\"";
		std::flush(std::cout);
    return;

  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "Message from Envelope::saveLightwaveEnvelope(std::string filename):" ;
		std::cout << std::endl << "             Writing \"" << filename <<  "\"";
		std::flush(std::cout);
  }

  writeToLightwaveMotion(filestream);

  filestream.close();
}


///////////////////////////////////////////////////////////////////////////////


  /// write envelope date to a newtek lightwave envelope or motion File

void
Envelope::writeToLightwaveMotion(std::ofstream& filestream)
{

  filestream << "{ Envelope" << std::endl;
  filestream << "  " << _keys.size() << std::endl;

  /// the could be a problem for you if you need higher precision
  /// I use lightwave conform length of 18
  filestream.precision(18);


  for(unsigned int i=0; i != _keys.size(); ++i)
  {
    filestream << "  Key " << _keys[i][0] << " "
                           << (_keys[i][1] / _framerate) << " "
                           << _keys[i][2] << " "
                           << _keys[i][3] << " "
                           << _keys[i][4] << " "
                           << _keys[i][5] << " "
                           << _keys[i][6] << " "
                           << _keys[i][7] << " "
                           << _keys[i][8] << std::endl;
  }


  filestream << "  Behaviors 1 1" << std::endl;
  filestream << "}" << std::endl;

  std::cout << std::endl;
  std::cout << std::endl << "Message from Envelope::writeToLightwaveMotion(std::string filename):" ;
  std::cout << std::endl << "             " << _keys.size() << " Keys written...";
  std::flush(std::cout);
}


///////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

void
Envelope::addKeyframe ( const Keyframe& key )
{
  if (_keys[_keys.size()-1].getTime() == key.getTime())
  {
    _keys[_keys.size()-1] = key;
    return;
  }

  _keys.push_back(key);
}


///////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

void
Envelope::addKeyframe ( const double& value, const double& time )
{
  if (_keys[_keys.size()-1].getTime() == time)
  {
    _keys[_keys.size()-1] = gloost::animation::Keyframe(value, time);
    return;
  }

  _keys.push_back(gloost::animation::Keyframe(value, time));
}


///////////////////////////////////////////////////////////////////////////////


 /// Get the value for a defined time linear interpolated

double
Envelope::getValueLinear ( double time )
{

  // What to do if we have to few keys
  if (_keys.empty())
  {
    return 0;
  }
  else if (_keys.size() < 2)
  {
    return _keys[0].getValue();
  }


  unsigned int k1 = findKeyframeBeforTime (time, 0, _keys.size());
  unsigned int k2 = k1+1;


  double t1, t2, v1, v2;

  t1 = _keys[k1].getTime();
  v1 = _keys[k1].getValue();


  if   ( k2 == _keys.size() )
  {
    k2 = k1;
    t2 =  time+1;
  }
  else
  {
    t2 = _keys[k2].getTime();
  }

  v2 = _keys[k2].getValue();

//  time = time-t1/(t2-t1);

  return (time-t1)*(v2-v1)/(t2-t1)+v1;
}


///////////////////////////////////////////////////////////////////////////////


/// Get the value for a defined time quadratic interpolate

//double
//Envelope::getValueQuadratic(double time )
//{
//
//  if (_keys.empty())
//  {
//    return 0;
//  }
//  else if (_keys.size() < 3)
//  {
//    return _keys[0].getValue();
//  }
//
//
//  // index for array acess
//  unsigned int i2 = this->findKeyframeBeforTime (time, 0, _keys.size());
//
//
//  double t1, t2, t3,
//         v1, v2, v3;
//
//  t2 = _keys[i2].getTime();
//  v2 = _keys[i2].getValue();
//
//  // key i1
//  if   ( ((int)i2)-1 < 0 )
//  {
//    t1 = _keys[i2].getTime()-1.0;
//    v1 = _keys[i2].getValue();
//  }
//  else
//  {
//    t1 = _keys[i2-1].getTime();
//    v1 = _keys[i2-1].getValue();
//  }
//
//  if   (i2+1 == _keys.size() )
//  {
//    t3 =  time+1;
//    v3 = _keys[_keys.size()-1].getValue();
//  }
//  else
//  {
//    t3 = _keys[i2+1].getTime();
//    v3 = _keys[i2+1].getValue();
//  }
//
//
//  /// fit quadratic function to three point
//  double n;
//  double a,  b,  c;
//
//
//  n=(t2-t1)*(t3-t1)*(t3-t2);
//
//  /// Magic
//  a=(-t2*v1+t3*v1
//      +t1*v2-t3*v2
//      -t1*v3+t2*v3) /n;
//  b=( t2*t2*v1 - t3*t3*v1
//      -t1*t1*v2 + t3*t3*v2
//      +t1*t1*v3 - t2*t2*v3) /n;
//  c=(-t2*t2*t3*v1+t2*t3*t3*v1
//      +t1*t1*t3*v2-t1*t3*t3*v2
//      -t1*t1*t2*v3+t1*t2*t2*v3) /n;
//
//  // Fast way of doing ap^2+bp+c
//  return (a*time+b)*time+c;
//}
//


///////////////////////////////////////////////////////////////////////////////


 /// Get the value for a defined time natural interpolate
//
//double
//Envelope::getValueNatural ( double time )
//{
//
// // What to do if we have to few keys
//  if (_keys.empty())
//  {
//    return 0;
//  }
//  else if (_keys.size() < 2)
//  {
//    return _keys[0].getValue();
//  }
//
////  if (time > _keys[_keys.size()-1].getTime())
////  {
////    time = time - _keys[_keys.size()-1].getTime();
////  }
//
//
//  unsigned int k0 = findKeyframeBeforTime (time, 0, _keys.size());
//  unsigned int k1;
//
//
//
//  double t1, t2, v1, v2;
//
//  t1 = _keys[k0].getTime();
//  v1 = _keys[k0].getValue();
//
//  if   ( k0+1 == _keys.size() )
//  {
//    k1 = k0;
//
//
//    t2 =  time+1;
//    v2 = _keys[_keys.size()-1].getValue();
//  }
//  else
//  {
//    t2 = _keys[k0+1].getTime();
//    v2 = _keys[k0+1].getValue();
//  }
//
//  double c1 = 0.005;
//  double c2 = 0.005;
//
//  double h, tmp;
//  h = t2 - t1;
//  tmp = c1/2.0 + ((time - t1)*(c2 - c1))/(6.0*h);
//  tmp = -(h/6.0)*(c2 + 2.0*c1) + (v2 - v1)/h + (time - t1)*tmp;
//  return v1 + (time - t1)*tmp;
//}


///////////////////////////////////////////////////////////////////////////////


/// Get the value for a defined time catmull rom interpolated
//    double getValueCatmullRom ( double time )
//{
//  Key* k1 = getEvaluableKeys (time, 3);
//  Key* k0 = k1->getPrevious();
//  Key* k2 = k1->getNext();
//  Key* k3 = k2->getNext();
//
//
//  double t0 = k0->getTime();
//  double y0 = k0->getValue();
//
//  double t1 = k1->getTime();
//  double y1 = k1->getValue();
//
//  double t2 = k2->getTime();
//  double y2 = k2->getValue();
//
//  double t3 = k3->getTime();
//  double y3 = k3->getValue();
//
//  double dt = (t2 - t1);       /* Time between se[i] and se[i+1] */
//  double u = (time - t1)/dt;   /* Fractional time from se[i] to p */
//  double dp0 = ((y1 - y0)/(t1 - t0) + (y2 - y1)/(t2 - t1))/2.0 * dt;
//  double dp1 = ((y3 - y2)/(t3 - t2) + (y2 - y1)/(t2 - t1))/2.0 * dt;
//
//  return (y1 * (2*u*u*u - 3*u*u + 1) +
//          y2 * (3*u*u - 2*u*u*u) +
//          dp0 * (u*u*u - 2*u*u + u) +
//          dp1 * (u*u*u - u*u) );
//}


///////////////////////////////////////////////////////////////////////////////


  /// Get the value for a defined time as Hermit (TCB-Spline)

//double
//Envelope::getValueTCB (double time)
//{
//  // What to do if we have to few keys
//  if (_keys.empty())
//  {
//    return 0;
//  }
//  else if (_keys.size() < 2)
//  {
//    return _keys[0].getValue();
//  }
//
//
//  unsigned int k1 = findKeyframeBeforTime (time, 0, _keys.size());
//  unsigned int k2 = k1+1;
//
//
//  double t1, t2, v1, v2;
//
//  t1 = _keys[k1].getTime();
//  v1 = _keys[k1].getValue();
//
//
//  if   ( k2 == _keys.size() )
//  {
//    k2 = k1;
//    t2 =  time+1;
//  }
//  else
//  {
//    t2 = _keys[k2].getTime();
//  }
//
//  v2 = _keys[k2].getValue();
//
//  float tension1 = _keys[k1][GLOOST_KEYFRAME_TENSION];
//  float tension2 = _keys[k2][GLOOST_KEYFRAME_TENSION];
//
//  float continu1 = _keys[k1][GLOOST_KEYFRAME_CONTINUITY];
//  float continu2 = _keys[k2][GLOOST_KEYFRAME_CONTINUITY];
//
//  float bias1    = _keys[k1][GLOOST_KEYFRAME_BIAS];
//  float bias2    = _keys[k2][GLOOST_KEYFRAME_BIAS];
//
//
//  /// calculation
//  float delta_t = (t2 - t1);
//  float delta_v = (v2 - v1);
//
//  float A = v1;
//  float B = delta_t*tension1;
//  float C = 3.0f*delta_v - delta_t*(2.0f*continu1 + tension2);
//  float D = -2.0f*delta_v - delta_t*(continu1 + tension2);
//
//
//  float timePosRatio = (time - t1)/delta_t;
//
//
//  float value = A
//              + B * timePosRatio
//              + C * timePosRatio*timePosRatio
//              + D * timePosRatio*timePosRatio*timePosRatio;
//
//  return value;
//}


///////////////////////////////////////////////////////////////////////////////


  /// returns the time of the last key

float
Envelope::getLastKeysTime()
{
  return _keys[_keys.size()-1][GLOOST_KEYFRAME_TIME];
}


///////////////////////////////////////////////////////////////////////////////


 /// find the keyframe for or befor the given time (binary search)

unsigned int
Envelope::findKeyframeBeforTime(double time, unsigned int startIndex, unsigned int endIndex)
{


  /// recursion end
  unsigned int size = endIndex - startIndex;

  if (size == 0)
  {
    return startIndex-1;
  }



  /// found key with key.time == time
  unsigned int middle = (unsigned int)((startIndex + endIndex)/2.0);

  if (_keys[middle].getTime() == time)
  {
    return middle;
  }


  if (time > _keys[middle].getTime() )
  {
    return findKeyframeBeforTime(time, middle+1, endIndex);
  }
  else
  {

    return findKeyframeBeforTime(time, startIndex, middle);
  }

}


///////////////////////////////////////////////////////////////////////////////


/// returns the key with index

Keyframe
Envelope::getKeyframe(unsigned int index) const
{
  if (index < _keys.size())
  {
    return _keys[index];
  }

  return Keyframe();
}


///////////////////////////////////////////////////////////////////////////////


 /// Get the number of keys stored

unsigned int
Envelope::getKeyframeCount()
{
  return _keys.size();
}


} // namespace animation
} // namespace gloost



