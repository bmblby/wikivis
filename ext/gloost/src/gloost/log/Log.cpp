
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
#include <gloost/log/Log.h>
#include <gloost/log/observer/ObserverBase.h>
#include <gloost/log/Message.h>


// cpp includes
#include <string>
#include <iostream>


namespace gloost
{
namespace log
{


/**
  \class   Log

  \brief   A Log supporting different kind of message types and log levels

  \author  Felix Weiszig, Dynardo GmbH
  \date    February 2015
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

Log::Log():
  _messagesMutex(),
  _messages(),
  _observersMutex(),
  _observers()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
Log::shared_ptr
Log::create()
{
	return shared_ptr(new Log());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Log::~Log()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
Log::addObserver(const std::shared_ptr<observer::ObserverBase>& observer)
{
  if (observer)
  {
    std::lock_guard<std::mutex> lock(_observersMutex);
    _observers.push_back(observer);
    observer->onBegin(shared_from_this());
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a lockguard locking the message container
*/

/*virtual*/
std::shared_ptr<std::lock_guard<std::mutex> >
Log::lockMessages()
{
  return std::shared_ptr<std::lock_guard<std::mutex> >(new std::lock_guard<std::mutex>(_messagesMutex));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief posts a message
*/

/*virtual*/
void
Log::post (const std::shared_ptr<Message>& message)
{
  {
    std::lock_guard<std::mutex> lock(_messagesMutex);
    _messages.push_back(message);
  }

  {
    std::lock_guard<std::mutex> lock(_observersMutex);
    for (const auto& observer : _observers)
    {
      observer->onPostMessage(message);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief posts a message
*/

/*virtual*/
const Log::MessageContainer&
Log::getMessages() const
{
  return _messages;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Message factory
*/

/*static*/
std::shared_ptr<Message>
Log::createMessage(const std::string& locationString, const std::string& text)
{
  return Message::create(LogLevel::MESSAGE, locationString, text);
}

/*static*/
std::shared_ptr<Message>
Log::createWarning(const std::string& locationString, const std::string& text)
{
  return Message::create(LogLevel::WARNING, locationString, text);
}

/*static*/
std::shared_ptr<Message>
Log::createDoh(const std::string& locationString, const std::string& text)
{
  return Message::create(LogLevel::DOH, locationString, text);
}

/*static*/
std::shared_ptr<Message>
Log::createDebug(const std::string& locationString, const std::string& text)
{
  return Message::create(LogLevel::DEBUG, locationString, text);
}

/*static*/
std::shared_ptr<Message>
Log::createNotification(const std::string& locationString, const std::string& text)
{
  return Message::create(LogLevel::NOTIFICATION, locationString, text);
}

/*static*/
std::shared_ptr<Message>
Log::createTesting(const std::string& locationString, const std::string& text)
{
  return Message::create(LogLevel::TESTING, locationString, text);
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

} // namespace log
} // namespace gloost


