
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
#include <gloost/templates/EmptyClass.h>

// cpp includes
#include <string>
#include <iostream>

namespace gloost
{
namespace subspace
{

/**
  \class   EmptyClass

  \brief   A class template because I'm to lazy to write a class from scratch

  \author  Felix Weiszig
  \date    May 2011
  \remarks Lazy mans way to create a new class ;-)
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

EmptyClass::EmptyClass()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   factory
  \remarks ...
*/

/*static*/
EmptyClass::shared_ptr
EmptyClass::create()
{
	return shared_ptr(new EmptyClass());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
EmptyClass::~EmptyClass()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the EmptyClass
  \param   ...
  \remarks ...
*/

void
EmptyClass::init()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

} // namespace subspace
} // namespace gloost

