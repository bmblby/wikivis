
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
#include <gloost/human_input/KeyboardState.h>

// cpp includes
#include <string>
#include <iostream>

namespace gloost
{
namespace human_input
{

/**
  \class   KeyboardState

  \brief   Mapps events to another

  \author  Felix Weiszig
  \date    April 2014
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

KeyboardState::KeyboardState():
  GenericSignalMapper()
{
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
KeyboardState::shared_ptr
KeyboardState::create()
{
	return shared_ptr(new KeyboardState());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
KeyboardState::~KeyboardState()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

} // namespace human_input
} // namespace gloost


