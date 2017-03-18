
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
#include <gloost/Rect.h>
#include <gloost/gloostMath.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   Rect

  \brief   ...

  \author  Felix Weiszig
  \date    May 2011
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Rect::Rect():
    _posX(0.0),
    _posY(0.0),
    _width(0.0),
    _height(0.0)
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Rect::Rect(const mathType& posX,  const mathType&  posY,
           const mathType& width, const mathType&  height):
    _posX(posX),
    _posY(posY),
    _width(width),
    _height(height)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Rect::Rect(const Rect& rect):
    _posX(rect._posX),
    _posY(rect._posX),
    _width(rect._posX),
    _height(rect._posX)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

Rect::~Rect()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

const mathType&
Rect::getPosX() const
{
	return _posX;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

const mathType&
Rect::getPosY() const
{
	return _posY;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

mathType
Rect::getPosX2() const
{
	return _posX + _width;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

mathType
Rect::getPosY2() const
{
	return _posY + _height;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

const mathType&
Rect::getWidth() const
{
	return _width;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

const mathType&
Rect::getHeight() const
{
	return _height;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


