
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
#include <gloost/log/LogLevel.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace log
{


/**
  \class   LogLevel

  \brief   Defines a class of log level identifiers

  \author  Felix Weiszig
  \date    Feb 2015
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////

  /*static*/ const LogLevel LogLevel::MESSAGE = LogLevel("Message");
  /*static*/ const LogLevel LogLevel::WARNING = LogLevel("Warning");
  /*static*/ const LogLevel LogLevel::DOH     = LogLevel("Doh");
  /*static*/ const LogLevel LogLevel::DEBUG   = LogLevel("Debug");
  /*static*/ const LogLevel LogLevel::NOTIFICATION = LogLevel("Notification");
  /*static*/ const LogLevel LogLevel::TESTING = LogLevel("Testing");
  /*static*/ const LogLevel LogLevel::POPUP   = LogLevel("POPUP");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

LogLevel::LogLevel(const std::string& levelString):
      _levelString(levelString)
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

bool
LogLevel::operator==(const LogLevel& loglevel) const
{
	return _levelString == loglevel._levelString;
}

////////////////////////////////////////////////////////////////////////////////


const std::string&
LogLevel::getString() const
{
  return _levelString;
}


////////////////////////////////////////////////////////////////////////////////

/*extern*/
std::ostream&
operator<<(std::ostream& os, const LogLevel& loglevel)
{
  if (loglevel == LogLevel::MESSAGE)
  {
    os << "Message";
  }
  else if (loglevel == LogLevel::WARNING)
  {
    os << "Warning";
  }
  else if (loglevel == LogLevel::DOH)
  {
    os << "DOH";
  }
  else if (loglevel == LogLevel::DEBUG)
  {
    os << "DEBUG";
  }
  else if (loglevel == LogLevel::NOTIFICATION)
  {
    os << "Notification";
  }
  else if (loglevel == LogLevel::TESTING)
  {
    os << "Testing";
  }
  else
  {
    os << "Babeling";
  }

  return os;
}




////////////////////////////////////////////////////////////////////////////////





} // namespace log
} // namespace gloost


