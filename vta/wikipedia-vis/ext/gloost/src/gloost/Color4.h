
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



#ifndef H_GLOOST_COLOR4
#define H_GLOOST_COLOR4



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>


namespace gloost
{


  //  Color with four components

class Color4
{
	public:

    // class constructor
    Color4();
    Color4(const Color4& color);
    Color4(const vec4& color);
    Color4(const vec3& color);
    Color4(const Vector3& color);
    Color4(float r, float g, float b, float a);

    // class destructor
	  ~Color4();


    // returns a reference to the red component
	  float& getR();
	  float& getG();
	  float& getB();
	  float& getA();


    float* getRgbaData();
    const float* getRgbaData() const;



	private:

   float _rgba[4];

};


} // namespace gloost


#endif // H_GLOOST_COLOR4


