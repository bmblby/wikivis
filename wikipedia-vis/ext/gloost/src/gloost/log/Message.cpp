
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
#include <gloost/log/Message.h>
#include <gloost/log/Log.h>
#include <gloost/util/Timer.h>
#include <gloost/gloostHelper.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace log
{


/**
  \class   Message

  \brief   A Message of the Log, Dynardo GmbH

  \author  Felix Weiszig
  \date    Feb 2015
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

Message::Message(const LogLevel&    logLevel,
                 const std::string& locationString,
                 const std::string& text):
    _time(gloost::util::Timer::getNow()),
    _logLevel(logLevel),
    _locationString(locationString),
    _stringStream()
{
  if (text.length())
  {
    _stringStream << text;
  }
}

////////////////////////////////////////////////////////////////////////////////

/*static*/
Message::shared_ptr
Message::create( const LogLevel&    logLevel,
                 const std::string& locationString,
                 const std::string& text)
{
	return shared_ptr(new Message(logLevel, locationString, text));
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
Message::~Message()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

std::stringstream&
Message::getStream()
{
	return _stringStream;
}

////////////////////////////////////////////////////////////////////////////////

const std::stringstream&
Message::getStream() const
{
	return _stringStream;
}

////////////////////////////////////////////////////////////////////////////////

const double&
Message::getTime() const
{
	return _time;
}

////////////////////////////////////////////////////////////////////////////////

const LogLevel&
Message::getLogLevel() const
{
	return _logLevel;
}

////////////////////////////////////////////////////////////////////////////////

const std::string&
Message::getLocationString() const
{
	return _locationString;
}

////////////////////////////////////////////////////////////////////////////////

std::string
Message::asString() const
{
	return _stringStream.str();
}

////////////////////////////////////////////////////////////////////////////////

std::string
Message::asFormatedString() const
{
	std::stringstream formatedMessage;
	formatedMessage << std::endl << _logLevel << " from \'" << _locationString << "\': " << std::endl;
  const auto levelStringLength = gloost::helper::toString(_logLevel).length();
  const auto fillString        = gloost::helper::stringRepeat(" ", levelStringLength+6u);
	const auto outString         = "  " + gloost::helper::stringSearchAndReplace(asString(), "\n", "\n" + fillString);
	formatedMessage << outString;
	return formatedMessage.str();
}

////////////////////////////////////////////////////////////////////////////////

/* extern */
std::ostream&
operator<< (std::ostream& os, const Message& message)
{
  os << message.asFormatedString();
  return os;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace log
} // namespace gloost


