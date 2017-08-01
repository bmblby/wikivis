
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


#ifndef H_GLOOST_SYSTEM_CONFIG
#define H_GLOOST_SYSTEM_CONFIG


/*
  In this file you can change some fundamental properties and behaviors of
  your gloost lib. Read the comments carefully and be sure you know what you
  are doing.
*/


/**
 \namespace  gloost
 \brief      namespace of the gloost framework
*/


// boost
#include <boost/filesystem/path.hpp>

// cpp includes
#include <iostream>



////////////////////////////////////////////////////////////////////////////////


  /*
     Here you can disable Messages, Warnings and Error Statements of gloost objects.
     Just uncomment the defines to disable console output.
  */

  #define GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES     /// <- let you know what gloost is doing
  #define GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS     /// <- occurs if something goes wrong
//  #define GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS       /// <- occurs if something is going eh ... wronger. maybe your app will crash
//  #define GLOOST_SYSTEM_DISABLE_OUTPUT_DEVNOTES     /// <- will give you information if a method is not implemented or obsolete


////////////////////////////////////////////////////////////////////////////////


  /*
     Here you can choose the type of gloost::mathType
     Choose wisely one of the defines.
     Type used by Point3, Vector3, Ray, Matrix, ... .
  */

#define GLOOST_SYSTEM_MATH_WITH_DOUBLE
//#define GLOOST_SYSTEM_MATH_WITH_FLOAT


////////////////////////////////////////////////////////////////////////////////


  /*
     if you use more than one GL contexts, for example on more than one gpus,
     you can adjust the number of render contexts gloost will handle up to 32
  */

#define GLOOST_SYSTEM_NUM_RENDER_CONTEXTS 1  /// <-- default: 1


////////////////////////////////////////////////////////////////////////////////


  /*
     for newer systems the path to freetype is /usr/include/freetype2
  */

// #define GLOOST_SYSTEM_FREETYPE2


////////////////////////////////////////////////////////////////////////////////



  /*
    This define will cause the gloost::Shader::disable method to reset all
    available texture units and set all texture matrices to identity. This
    adds a certain amount of overhead to your cpu time every time a shader
    is disabled. On the pro side, the state of your texture units is always
    defined when disabling a shader.

    You have to keep track of your texture state yourselfe if you remove this
    define. In many cases this will be trivial, especially if you dont use
    texture matrices other then identity
  */

#define GLOOST_SYSTEM_CLEAR_TEXUNITS_ON_SHADER_DISABLE


////////////////////////////////////////////////////////////////////////////////

/// BETTER DONT CHANGE ANYTHING BELOW THIS LINE ////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

namespace gloost
{

/**
  On several points in the framework custom ids are used instead of resource handles
  provided by the OpenGL. I.e. the gloost::TextureManager will give you an unique
  id for each texture ressource it stores. This id can be used to get the actual
  gloost::Texture from the TextureManager as well as pass the texture ressource
  to a shader using the gl::UniformSet.
  To distinguish this ids from OpenGL ressource handles all ids provided by gloost
  are of type gloostId. OpenGL resource handles are described and named through out
  the framework as "handle". This is a bit inconsistent by now since I just started
  to rename things.
*/
typedef int gloostId; //old
typedef int gid;      //new

#ifdef GLOOST_SYSTEM_MATH_WITH_FLOAT

  /// the math type used in all math related classes around the framework
  typedef float mathType;

  /// as seen here: http://cplus.about.com/od/learning1/ss/clessonone_6.htm
  #define GLOOST_MATHTYPE_MIN_EPSILON 1e-6

#endif

#ifdef GLOOST_SYSTEM_MATH_WITH_DOUBLE

  /// ...
  typedef double mathType;

  /// as seen here: http://cplus.about.com/od/learning1/ss/clessonone_6.htm
  #define GLOOST_MATHTYPE_MIN_EPSILON 1e-15

#endif


  typedef boost::filesystem::path PathType;

////////////////////////////////////////////////////////////////////////////////


  /// this functions prints some information about the configuration of your gloost
inline
void
printSystemInfo()
{
  std::cout << std::endl;
  std::cout << std::endl << "";


  std::cout << std::endl << "                  ___                            __";
  std::cout << std::endl << "                 /\"_ \"                          /\" \"__";
  std::cout << std::endl << "              __ \"//\" \"     ___     ___     ____\" \"  _\"";
  std::cout << std::endl << "            /'_ `\" \" \" \"   / __`\"  / __`\"  /  __\"\" \" \"/";
  std::cout << std::endl << "           /\" \" \" \" \"_\" \"_/\" \" \" \"/\" \" \" \"/\"__   \"\" \" \"_";
  std::cout << std::endl << "           \" \"____ \"/\"____\" \"____/\" \"____/\"/\"____/ \" \"__\"";
  std::cout << std::endl << "            \"/___/\" \"/____/\"/___/  \"/___/  \"/___/   \"/__/";
  std::cout << std::endl << "              /\"____/";
  std::cout << std::endl << "              \"_/__/";
  std::cout << std::endl << "";
  std::cout << std::endl << "              OpenGL framework for fast demo programming";
  std::cout << std::endl << "";
  std::cout << std::endl << "                        http://www.gloost.org";


  std::cout << std::endl << "########## gloost system info ##########";
  std::cout << std::endl << "";
  std::cout << std::endl << "gloost math works with ......... ";

#ifdef GLOOST_SYSTEM_MATH_WITH_DOUBLE
  std::cout << "double";
#endif
#ifdef GLOOST_SYSTEM_MATH_WITH_FLOAT
  std::cout << "float";
#endif

  std::cout << std::endl << "";
  std::cout << std::endl << "messages are ......... ";

#ifdef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout << "off";
#endif

  std::cout << std::endl;
}




} /// namespace gloost



#endif /// GLOOST_SYSTEM_CONFIG_H
