
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



#ifndef H_GLOOST_LOG_LOGLEVEL
#define H_GLOOST_LOG_LOGLEVEL



// gloost system includes
#include <gloost/gloostConfig.h>


// cpp includes
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>



namespace gloost
{
namespace log
{

  // Defines a class of log level identifiers

class LogLevel
{
  public:
    LogLevel(const std::string& levelName);
    static const LogLevel MESSAGE;
    static const LogLevel WARNING;
    static const LogLevel DOH;
    static const LogLevel DEBUG;
    static const LogLevel NOTIFICATION;
    static const LogLevel TESTING;
    static const LogLevel POPUP;

    bool operator==(const LogLevel&) const;
    const std::string& getString() const;

  private:
    std::string _levelString;

};


extern std::ostream& operator<<(std::ostream&, const LogLevel&);

} // namespace log
} // namespace gloost



#endif // H_GLOOST_LOG_LOGLEVEL


