
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
#include <gloost/log/observer/ObserverBase.h>
#include <gloost/log/Log.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace log
{
namespace observer
{


/**
  \class   ObserverBase

  \brief   Base of all Log Observer

  \author  Felix Weiszig, Dynardo GmbH
  \date    Feb 2015
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

ObserverBase::ObserverBase()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   factory
  \remarks ...
*/

/*static*/
ObserverBase::shared_ptr
ObserverBase::create()
{
	return shared_ptr(new ObserverBase());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
ObserverBase::~ObserverBase()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ObserverBase::onBegin (const std::shared_ptr<Log>&)
{

}


////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ObserverBase::onPostMessage (const std::shared_ptr<const Message>&)
{

}


////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ObserverBase::onEnd (const std::shared_ptr<Log>&)
{

}

////////////////////////////////////////////////////////////////////////////////





} // namespace observer
} // namespace log
} // namespace gloost


