
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



#ifndef H_GLOOST_LOG_OBSERVER_TOXMLFILE
#define H_GLOOST_LOG_OBSERVER_TOXMLFILE



// gloost system includes
#include <gloost/log/observer/ObserverBase.h>
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>
#include <mutex>



namespace gloost
{
namespace log
{

namespace observer
{


  //  TerminalOutput template

class TerminalOutput : public ObserverBase
{
	public:

    /// a std::shared_ptr of an TerminalOutput instance
    typedef std::shared_ptr<TerminalOutput>       shared_ptr;
    typedef std::shared_ptr<const TerminalOutput> const_shared_ptr;

    // class factory
    static shared_ptr create();

    // class destructor
	  virtual ~TerminalOutput();

	  virtual void onBegin       (const std::shared_ptr<Log>& log);
	  virtual void onPostMessage (const std::shared_ptr<const Message>& message);
    virtual void onEnd         (const std::shared_ptr<Log>& log);

	protected:

    // class constructor
    TerminalOutput();


	private:

   unsigned   _messageCounter;
   std::mutex _terminalMutex;

};


} // namespace observer
} // namespace log
} // namespace gloost


#endif // H_GLOOST_LOG_OBSERVER_TOXMLFILE


