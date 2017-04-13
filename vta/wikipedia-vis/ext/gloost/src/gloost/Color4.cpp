
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
#include <gloost/Color4.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   Color4

  \brief   A class template because I'm to lazy to write a class from scretch

  \author  Felix Weiszig
  \date    May 2011
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Color4::Color4():
  _rgba()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Color4::Color4(const Color4& color):
  _rgba()
{
  memcpy(this, &color, sizeof(Color4));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Color4::Color4(const vec4& color):
  _rgba()
{
	_rgba[0] = color.r;
	_rgba[1] = color.g;
	_rgba[2] = color.b;
	_rgba[3] = color.a;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Color4::Color4(const vec3& color):
  _rgba()
{
	_rgba[0] = color.x;
	_rgba[1] = color.y;
	_rgba[2] = color.z;
	_rgba[3] = 1.0;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Color4::Color4(const Vector3& color):
  _rgba()
{
	_rgba[0] = color[0];
	_rgba[1] = color[1];
	_rgba[2] = color[2];
	_rgba[3] = 1.0;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Color4::Color4(float r, float g, float b, float a):
  _rgba()
{
  _rgba[0] = r;
	_rgba[1] = g;
	_rgba[2] = b;
	_rgba[3] = a;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

Color4::~Color4()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief    returns a pointer to the four float components
  \param   ...
  \remarks ...
*/

float*
Color4::getRgbaData()
{
	return _rgba;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a const pointer to the four float components
  \param   ...
  \remarks ...
*/

const float*
Color4::getRgbaData() const
{
	return _rgba;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


