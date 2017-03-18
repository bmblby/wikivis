
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



#ifndef GLOOST_ENVELOPE_H
#define GLOOST_ENVELOPE_H



// gloost system includes
#include <gloost/animation/Keyframe.h>



// cpp includes
#include <string>
#include <vector>



namespace gloost
{
namespace animation
{


  ///  Provides you with path animation functionality based on keyframes

class Envelope
{
	public:
    /// class constructor
    Envelope(float framerate = 60);

    /// class destructor
    ~Envelope();



    /// Add a Keyframe to the Envelope
    void addKeyframe ( const Keyframe& key );

    /// Add a Keyframe to the Envelope
    void addKeyframe ( const double& value, const double& time );



    /// clears the envelope and ads a key at time 0 with value 0
    void clear();



    /// load a newtek lightwave envelope File
    void loadLightwaveEnvelope( std::string filename, float framerate = 60.0f);

    /// save a newtek lightwave envelope File
    void saveLightwaveEnvelope( std::string filename);

    /// write envelope date to a newtek lightwave envelope or motion File
    void writeToLightwaveMotion(std::ofstream& filestream);



    /// Get the value for a defined time linear interpolated
    double getValueLinear (double time);

    /// Get the value for a defined time quadratic interpolated
    double getValueQuadratic (double time);

    /// Get the value for a defined time natural interpolated
    //double getValueNatural (double time);


//    /// Get the value for a defined time catmull rom interpolated
//    double getValueCatmullRom ( double time );
//
    /// Get the value for a defined time as Hermit (TCB-Spline)
//    double getValueTCB     ( double time );



    /// returns the time of the last key
    float getLastKeysTime();


    /// returns the key with index
    Keyframe getKeyframe(unsigned int index) const;


    /// Get the number of keys stored
    unsigned int getKeyframeCount();


    /// find the keyframe for or befor the given time (binary search)
    unsigned int findKeyframeBeforTime(double time, unsigned int startIndex, unsigned int endIndex);


	private:

    ///
    std::string _filename;

    /// list of keys
    std::vector<Keyframe> _keys;

    /// frame rate to scale the time values
    float _framerate;
};


} // namespace animation
} // namespace gloost


#endif // GLOOST_ENVELOPE_H

