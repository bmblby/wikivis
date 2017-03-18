
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



#ifndef GLOOST_KEYFRAME_H
#define GLOOST_KEYFRAME_H



// cpp includes
#include <iostream>


/// defines for the different components of a keyframe definition
#define GLOOST_KEYFRAME_VALUE      0
#define GLOOST_KEYFRAME_TIME       1
#define GLOOST_KEYFRAME_NONE1      2
#define GLOOST_KEYFRAME_TENSION    3
#define GLOOST_KEYFRAME_CONTINUITY 4
#define GLOOST_KEYFRAME_BIAS       5
#define GLOOST_KEYFRAME_NONE2      6
#define GLOOST_KEYFRAME_NONE3      7
#define GLOOST_KEYFRAME_NONE4      8



namespace gloost
{
namespace animation
{



///  This class hold all informations for an keyframe used by an Envelope

class Keyframe
{
	public:
    /// class constructor
    Keyframe ();

    /// class constructor
    Keyframe ( double value, double time );

    /// class constructor
    Keyframe ( double value,
               double time,
               double v1,
               double tension,
               double continuity,
               double bias,
               double v2,
               double v3,
               double v4 );

    /// class constructor
    Keyframe (const Keyframe& key );

    /// class destructor
    ~Keyframe ();


    /// indexing
    double operator[](unsigned int a) const;

    /// set all values to 0
    void reset ();

    /// get the value
    void   setValue ( double value );
    double getValue () const;

    /// get the time
    void   setTime ( double value );
    double getTime () const;


	private:

    /// Keyframe information
    double _key[ 9 ];



};


} /// namespace animation
} /// namespace gloost

extern std::ostream& operator<< (std::ostream&, const gloost::animation::Keyframe&);


#endif // GLOOST_KEYFRAME_H

