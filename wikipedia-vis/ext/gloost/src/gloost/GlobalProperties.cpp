
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


// vxfv system includes
#include <gloost/GlobalProperties.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class GlobalProperties

  \brief A PropertySet as singelton for application wide variables and configurations

  \author Felix Weiszig
  \date   May 2011
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/*static*/ GlobalProperties* GlobalProperties::_theInstance = 0;


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor

  \remarks ...
*/

GlobalProperties::GlobalProperties():
  PropertySet()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
  \remarks ...
*/

GlobalProperties::~GlobalProperties()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief returns an instance of this class
  \remarks ...
*/

/*static*/
GlobalProperties* GlobalProperties::get()
{
  if (!_theInstance)
  {
    _theInstance = new GlobalProperties();
  }

  return _theInstance;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


