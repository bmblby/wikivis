
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



#ifndef H_GLOOST_LOG_MESSAGE
#define H_GLOOST_LOG_MESSAGE



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/log/LogLevel.h>


// cpp includes
#include <string>
#include <sstream>
#include <memory>

// boost
#include <boost/current_function.hpp>



namespace gloost
{
namespace log
{

  //  Message template

class Message
{
	public:

    /// a std::shared_ptr of an Message instance
    typedef std::shared_ptr<Message>       shared_ptr;
    typedef std::shared_ptr<const Message> const_shared_ptr;

    // class factoy
    static shared_ptr create(const LogLevel&    logLevel,
                             const std::string& locationString,
                             const std::string& text = "");

    // class destructor
	  virtual ~Message();

    std::stringstream&       getStream();
    const std::stringstream& getStream() const;

    const double&      getTime()       const;
    const LogLevel&    getLogLevel() const;
    const std::string& getLocationString() const;

	  std::string asString() const;
	  std::string asFormatedString() const;

	protected:

    // class constructor
    Message( const LogLevel&    logLevel,
             const std::string& locationString,
             const std::string& text = "");

	private:

	  double             _time;
	  LogLevel           _logLevel;
	  std::string        _locationString;
	  std::stringstream  _stringStream;



};

/// ostream operator
extern std::ostream& operator<< (std::ostream&, const Message&);


} // namespace log
} // namespace gloost



#endif // H_GLOOST_LOG_MESSAGE


