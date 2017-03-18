
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



#ifndef H_GLOOST_VALUE_TO_WAVELENGTH
#define H_GLOOST_ValueToWavelength



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>



namespace gloost
{
namespace util
{


  //  converts a value between min...max to a color of the visible spectrum

class ValueToWavelength
{
	public:

    // class constructor
    ValueToWavelength() = delete;

    // class destructor
	  ~ValueToWavelength() = delete;


    // converts a value between min...max to a color of the visible spectrum
	  static vec4 toWavelengthColor(float value, float min, float max);


	protected:

   // ...


	private:

    static
    float getWaveLengthFromDataPoint(float value, float min, float max);

    static
    vec4 wavelengthToRGB(float wavelength);

    static
    unsigned adjust(float colorComponent, float factor);


};


} // namespace util
} // namespace gloost


#endif // H_GLOOST_ValueToWavelength


