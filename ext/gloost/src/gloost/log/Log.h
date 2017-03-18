
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



#ifndef H_GLOOST_LOG_LOG
#define H_GLOOST_LOG_LOG



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
  class Message;
  namespace observer {class ObserverBase;}

class Log : public std::enable_shared_from_this<Log>
{
	public:

    /// a std::shared_ptr of an Log instance
    typedef std::shared_ptr<Log>                         shared_ptr;
    typedef std::shared_ptr<const Log>                   const_shared_ptr;
    typedef std::vector<std::shared_ptr<const Message> > MessageContainer;

    static shared_ptr create();
	  virtual ~Log();

	  void addObserver(const std::shared_ptr<observer::ObserverBase>& observer);

    // message factories
	  static std::shared_ptr<Message> createMessage(const std::string& locationString,      const std::string& text = "");
	  static std::shared_ptr<Message> createWarning(const std::string& locationString,      const std::string& text = "");
	  static std::shared_ptr<Message> createDoh    (const std::string& locationString,      const std::string& text = "");
	  static std::shared_ptr<Message> createDebug  (const std::string& locationString,      const std::string& text = "");
	  static std::shared_ptr<Message> createNotification(const std::string& locationString, const std::string& text = "");
	  static std::shared_ptr<Message> createTesting(const std::string& locationString,      const std::string& text = "");



	  // posts a message
	  virtual void post (const std::shared_ptr<Message>& message);

    // returns a lockguard locking the message container
	  std::shared_ptr<std::lock_guard<std::mutex> > lockMessages();
	  const MessageContainer& getMessages() const;


	protected:

    // class constructor
    Log();


	private:

	  std::mutex       _messagesMutex;
    MessageContainer _messages;

	  std::mutex                                            _observersMutex;
    std::vector<std::shared_ptr<observer::ObserverBase> > _observers;

};


} // namespace log
} // namespace gloost



#endif // H_GLOOST_LOG_LOG


