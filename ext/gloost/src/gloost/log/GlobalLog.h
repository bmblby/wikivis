
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



#ifndef H_GLOOST_LOG_GLOBALLOG
#define H_GLOOST_LOG_GLOBALLOG



// gloost system includes
#include <gloost/log/Log.h>


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

  // Singleton version of a gloost::log::Log

class GlobalLog : public Log
{
	public:

    typedef std::shared_ptr<GlobalLog>       shared_ptr;
    typedef std::shared_ptr<const GlobalLog> const_shared_ptr;

    virtual ~GlobalLog();

    static shared_ptr get();

    // posts messages to the global log
    static void postMessage      (const std::string& locationString, const std::string& messageText);
    static void postWarning      (const std::string& locationString, const std::string& messageText);
    static void postDoh          (const std::string& locationString, const std::string& messageText);
    static void postDebug        (const std::string& locationString, const std::string& messageText);
    static void postNotification (const std::string& locationString, const std::string& messageText);
    static void postTesting      (const std::string& locationString, const std::string& messageText);


	protected:

    // class constructor
    GlobalLog();


	private:

	  static shared_ptr _instance;

};

} // namespace log
} // namespace gloost



#endif // H_GLOOST_LOG_GLOBALLOG


