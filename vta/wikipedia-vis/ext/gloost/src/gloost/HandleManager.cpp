
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
#include <gloost/HandleManager.h>

// cpp includes
#include <string>
#include <iostream>

namespace gloost
{

/**
  \class   HandleManager

  \brief   Provides system wide unique ids/handles

  \author  Felix Weiszig
  \date    September 2016
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////

/*static*/ HandleManager* HandleManager::_instance = nullptr;

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   factory
  \remarks ...
*/

/*static*/
//HandleManager::shared_ptr
HandleManager*
HandleManager::get()
{
  if (!_instance)
  {
//    _instance = shared_ptr(new HandleManager());
    _instance = new HandleManager();
  }
	return _instance;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

HandleManager::HandleManager():
  _handleCounter(1),
  _handleStack()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
HandleManager::~HandleManager()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the HandleManager
  \param   ...
  \remarks ...
*/

HandleManager::HandleType
HandleManager::grabHandle()
{
  std::lock_guard<std::mutex> lock(_handleHandleMutex);

  if (_handleStack.size())
  {
    const auto handle = _handleStack.top();
    _handleStack.pop();
    return handle;
  }
  return ++_handleCounter;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the HandleManager
  \param   ...
  \remarks ...
*/

void
HandleManager::releaseHandle(HandleType handle)
{
  std::lock_guard<std::mutex> lock(_handleHandleMutex);
  _handleStack.push(handle);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the number of handles currently grabbed
  \param   ...
  \remarks ...
*/

size_t
HandleManager::getNumGrabbedHandles() const
{
  return (_handleCounter-_handleStack.size());
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \param   ...
  \remarks ...
*/

HandleGuard::HandleGuard():
  _handle(HandleManager::get()->grabHandle())
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   destroyes the guard and releases the handle
  \param   ...
  \remarks ...
*/

HandleGuard::~HandleGuard()
{
  HandleManager::get()->releaseHandle(_handle);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the actual handle
  \param   ...
  \remarks ...
*/
const HandleManager::HandleType&
HandleGuard::getHandle() const
{
  return _handle;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost

