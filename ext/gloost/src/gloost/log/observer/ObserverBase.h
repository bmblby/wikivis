
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



#ifndef H_GLOOST_LOG_OBSERVER_OBSERVERBASE
#define H_GLOOST_LOG_OBSERVER_OBSERVERBASE



// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{
namespace log
{
  class Message;
  class Log;

namespace observer
{


  //  ObserverBase template

class ObserverBase
{
	public:

    /// a std::shared_ptr of an ObserverBase instance
    typedef std::shared_ptr<ObserverBase>       shared_ptr;
    typedef std::shared_ptr<const ObserverBase> const_shared_ptr;

    // class factory
    static shared_ptr create();

    // class destructor
	  virtual ~ObserverBase();

	  virtual void onBegin       (const std::shared_ptr<Log>& log);
	  virtual void onPostMessage (const std::shared_ptr<const Message>& message);
    virtual void onEnd         (const std::shared_ptr<Log>& log);

	protected:

    // class constructor
    ObserverBase();


	private:

   // ...

};


} // namespace observer
} // namespace log
} // namespace gloost


#endif // H_GLOOST_LOG_OBSERVER_OBSERVERBASE


