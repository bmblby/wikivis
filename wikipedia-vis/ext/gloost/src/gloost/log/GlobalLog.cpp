
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
#include <gloost/log/GlobalLog.h>
#include <gloost/log/observer/ObserverBase.h>
#include <gloost/log/observer/TerminalOutput.h>


// cpp includes
#include <string>
#include <iostream>


namespace gloost
{
namespace log
{


/**
  \class   GlobalLog

  \brief   Singleton version of a gloost::log::Log

  \author  Felix Weiszig, Dynardo GmbH
  \date    February 2015
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

 std::shared_ptr<GlobalLog> GlobalLog::_instance = nullptr;

////////////////////////////////////////////////////////////////////////////////

GlobalLog::GlobalLog()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
GlobalLog::~GlobalLog()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
GlobalLog::shared_ptr
GlobalLog::get()
{
  if (!_instance)
  {
    _instance = std::shared_ptr<GlobalLog>(new GlobalLog());
    _instance->addObserver(observer::TerminalOutput::create());
  }
  return _instance;
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
void
GlobalLog::postMessage(const std::string& locationString, const std::string& messageText)
{
  get()->post(createMessage(locationString, messageText));
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
void
GlobalLog::postWarning(const std::string& locationString, const std::string& messageText)
{
  get()->post(createWarning(locationString, messageText));
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
void
GlobalLog::postDoh(const std::string& locationString, const std::string& messageText)
{
  get()->post(createDoh(locationString, messageText));
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
void
GlobalLog::postDebug(const std::string& locationString, const std::string& messageText)
{
  get()->post(createDebug(locationString, messageText));
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
void
GlobalLog::postNotification(const std::string& locationString, const std::string& messageText)
{
  get()->post(createNotification(locationString, messageText));
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
void
GlobalLog::postTesting(const std::string& locationString, const std::string& messageText)
{
  get()->post(createTesting(locationString, messageText));
}

////////////////////////////////////////////////////////////////////////////////

} // namespace log
} // namespace gloost


